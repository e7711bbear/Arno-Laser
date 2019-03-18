#define FAN_CTRL      4
#define FAN_PWM       5

#define WATER_CTRL    8
#define WATER_PWM     10
#define WATER_THERMO  37
#define WATER_FLOW    36

#define AIR_CTRL      22

#define I2C_SDA       20
#define I2C_SCL       21
#define I2C_SLAVE     0x8



typedef enum {
  none = 0,
  prod = 1,
  dev = 2
} DebugLevel;

typedef struct {
  DebugLevel laser;
  
  DebugLevel air;
  
  DebugLevel water;
  DebugLevel fans;
  DebugLevel thermometer;
  DebugLevel flowmeter;
  
  DebugLevel i2c;
} DebugConfig;

DebugConfig debugConfig = {
  .laser = dev,

  .air = dev,

  .water = dev,
  .fans = dev,
  .thermometer = dev,
  .flowmeter = dev,

  .i2c = dev
};

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>

void setup() {
  setupDebugging();

  setupFans();
  setupWaterSystem();
  setupAirPump();

  setupI2C();
}

void loop() {
  static int index = 0;
  float temperature = readWaterThermometer();
  
  Serial.print("Temperature: ");
  Serial.println(temperature);

  delay(100); // loop at 10Hz
}


//
//  Fans Controls
//

void setupFans() {
  pinMode(FAN_CTRL, OUTPUT);
  pinMode(FAN_PWM, OUTPUT);

  turnOnFans();
  setFanSpeed(100);
}

void turnOnFans() {
  digitalWrite(FAN_CTRL, HIGH);
}

void turnOffFans() {
  digitalWrite(FAN_CTRL, LOW);
}

void setFanSpeed(int speed) { // 0-100%
  speed = map(speed, 0, 100, 0, 255);
  analogWrite(FAN_PWM, speed);
}

//
// Water Pump Controls
//

void setupWaterSystem() {
  pinMode(WATER_CTRL, OUTPUT);
  pinMode(WATER_PWM, OUTPUT);
  pinMode(WATER_THERMO, INPUT);
}

void turnOnWaterPump() {
  digitalWrite(WATER_CTRL, HIGH);
}

void turnOffWaterPump() {
  digitalWrite(WATER_CTRL, LOW);
}

OneWire waterThermoWire(WATER_THERMO);
DallasTemperature waterThermometer(&waterThermoWire);

float readWaterThermometer() {
  waterThermometer.requestTemperatures();
  return waterThermometer.getTempCByIndex(0);
}


//
// Air Pump Controls
// 

void setupAirPump() {
  pinMode(AIR_CTRL, OUTPUT);
}

void turnOnAirPump() {
  digitalWrite(AIR_CTRL, HIGH);
}

void turnOffAirPump() {
  digitalWrite(AIR_CTRL, LOW);
}

//
// I2C
//

// Cmds list
// 
// AirTurnOn:     0x10
// AirTurnOff:    0x11
//
// FansTurnOn:    0x20
// FansTurnOff:   0x21
//
// WaterTurnOn:   0x30
// WaterTurnOff:  0x31
//

void setupI2C() {
  Wire.begin(I2C_SLAVE);
  Wire.onReceive(receivedI2C);
  Wire.onRequest(requestedPush);
}

void receivedI2C() {
  while(Wire.available()) {
    int data = Wire.read();

    Serial.print("I2C received: ");
    Serial.println(data);
  }
}

void sendToI2C() {
  Wire.beginTransmission(0x00);
  uint8_t *value = 0;
  size_t len = 1;
  Wire.write(value, len);
  Wire.endTransmission();
}

// Push Data pack

typedef struct {
  bool fanState = 0; // 0 off, 1 on
  bool airState = 0;
  bool waterState = 0;
  int temp; // celsius, x100 from float
} pushPack;

void requestedPush() {
  pushPack pack;

  float tempTemp = readWaterThermometer();
  pack.temp = tempTemp * 100;
  
  Wire.write((uint8_t*)&pack, sizeof(pack));
}

//
// Debugging
//

void setupDebugging() {
  Serial.begin(115200);
  // global was set before 
}

// add here I2C cmds for activating debugging.
