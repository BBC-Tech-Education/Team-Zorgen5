#include <motors.h>
#include <out.h>
#include <pins.h>
#include <Orbit.h>
#include <constants.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_Sensor.h>
#include <PID.h>
#include <Arduino.h>

Motors gorobotgo;
Motors move(int direction, int correction, int speed);
Out out;
Orbit orbit;
PID correction;
PID update(float input, float setpoint);
int targetHeading = 0;
Adafruit_BNO055 bno = Adafruit_BNO055(55, BNO055_ADDRESS_B, &Wire1);


void setup() {
  Serial.begin(9600);
  bno.begin(OPERATION_MODE_IMUPLUS);
  while(!bno.begin(OPERATION_MODE_IMUPLUS)) {
    Serial.println("No BNO055 detected. Check your wiring or I2C ADDR.");
    delay(1000);
  }
  gorobotgo.init();
  out.init();
  orbit.init();
  bno.setExtCrystalUse(true);
  


}
//imuEvent
void loop() {
  sensors_event_t compass;
  bno.getEvent(&compass);
  float heading = float(compass.orientation.x);
  heading = targetHeading - heading;
  //Serial.print(heading);
  if (heading > 180.0f) {
      heading -= 360.0f;
  }


  float dir = orbit.orbit();
  float rotation = correction.update(heading, targetHeading);

  gorobotgo.move(0.0, rotation, 0.0);
  Serial.print(rotation);
  Serial.println();


  
  // digitalWrite(INA_1, HIGH);
  // digitalWrite(INB_1, LOW);
  // analogWrite(PWM_1, 25);

  // digitalWrite(INA_2, HIGH);
  // digitalWrite(INB_2, LOW);
  // analogWrite(PWM_2, 25);

  // digitalWrite(INA_3, HIGH);
  // digitalWrite(INB_3, LOW);
  // analogWrite(PWM_3, 25);

  // digitalWrite(INA_4, HIGH);
  // digitalWrite(INB_4, LOW);
  // analogWrite(PWM_4, 25);
  



}  