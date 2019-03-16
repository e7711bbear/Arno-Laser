#define FAN_CTRL    4
#define FAN_PWM     5

#define WATER_CTRL  8
#define WATER_PWM   10

#define AIR_CTRL    22

#define I2C_SDA     20
#define I2C_SCL     21
#define I2C_SLAVE   8

#define FLOW        36

#define THERMO      37

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
  setupWaterPump();
  setupAirPump();
  setupThermometer();
}

void loop() {
  static int index = 0;
  // put your main code here, to run repeatedly:
  float temperature = readThermometer();
  Serial.print("Temperature: ");
  Serial.println(temperature);

  delay(100); // loop at 10Hz

  index++;

  if (index <= 20) {
    turnOnAirPump();
  } else {
    turnOffAirPump();
  }
  index = index >= 40 ? 0 : index;
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

void setupWaterPump() {
  pinMode(WATER_CTRL, OUTPUT);
  pinMode(WATER_PWM, OUTPUT);

  turnOnWaterPump();
}

void turnOnWaterPump() {
  digitalWrite(WATER_CTRL, HIGH);
}

void turnOffWaterPump() {
  digitalWrite(WATER_CTRL, LOW);
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
// Thermometer controls
//

OneWire thermoWire(THERMO);
DallasTemperature thermometer(&thermoWire);

void setupThermometer() {
  pinMode(THERMO, INPUT);
}

float readThermometer() {
  thermometer.requestTemperatures();
  return thermometer.getTempCByIndex(0);
}


//
// I2C
//

void setupI2C() {
  Wire.begin(I2C_SLAVE);
  Wire.onReceive(receivedI2C);
}

void receivedI2C() {
  while(Wire.available()) {
    int data = Wire.read();

    Serial.print("I2C received: ");
    Serial.println(data);
  }
}


//
// Debugging
//

void setupDebugging() {
  Serial.begin(115200);
  // global was set before 
}

// add here I2C cmds for activating debugging.
