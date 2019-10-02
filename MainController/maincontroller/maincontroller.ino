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
//  setupAirPump();

//  setupI2C();
}

void loop() {
  static int index = 0;
  float temperature = readWaterThermometer();
  
//  Serial.print("Temperature: ");
//  Serial.println(temperature);

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
  Serial.println("Turning Fans On");
  digitalWrite(FAN_CTRL, HIGH);
}

void turnOffFans() {
  Serial.println("Turning Fans Off");
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
  turnOnWaterPump();
}

void turnOnWaterPump() {
  Serial.println("Turning Water Pump On");
  digitalWrite(WATER_CTRL, HIGH);
  digitalWrite(WATER_PWM, HIGH);
}

void turnOffWaterPump() {
  Serial.println("Turning Water Pump Off");
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
  Serial.println("Turning Air Pump On");
  digitalWrite(AIR_CTRL, HIGH);
}

void turnOffAirPump() {
  Serial.println("Turning Air Pump Off");
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
//  Wire.onRequest(requestedPush);
}

void receivedI2C() {
  int buffer[10];
  int index = 0;
  
  while(Wire.available()) {
    int data = Wire.read();

    Serial.print("I2C received: ");
    Serial.println(data);
    buffer[index++] = data;
  }

  Serial.println("Passing to I2C handler");
  handleI2CCommand(buffer[0], buffer[1]);
}

void sendToI2C() {
  Wire.beginTransmission(0x00);
  uint8_t *value = 0;
  size_t len = 1;
  Wire.write(value, len);
  Wire.endTransmission();
}

void handleI2CCommand(int cmd, int param) {
  switch(cmd) {
    case 0x10:
      turnOnAirPump();
      return;
    
    case 0x11:
      turnOffAirPump();
      return;
    
    case 0x20:
      turnOnFans();
      return;

    case 0x21:
      turnOffFans();
      return;

    case 0x30:
      turnOnWaterPump();
      return;

    case 0x31:
      turnOffWaterPump();
      return;
    
    default:
      return;
  }
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
