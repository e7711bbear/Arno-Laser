#define FAN_CTRL    4
#define FAN_PWM     5

#define WATER_CTRL  8
#define WATRR_PWM   10

#define AIR_CTRL    22

#define I2C_1       20
#define I2C_2       21

#define FLOW        36

#define THERMO      37

#include <OneWire.h>
#include <DallasTemperature.h>

void setup() {
  Serial.begin(115200);

  setupFans();
  setupWaterPump();
  setupThermometer();
}

void loop() {
  // put your main code here, to run repeatedly:
  float temperature = readThermometer();
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

void setupWaterPump() {
  
}

void turnOnWaterPump() {
  
}

void turnOffWaterPump() {

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


