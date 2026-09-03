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
Out out;
Orbit orbit;
PID correction(IMU_KP, IMU_KI, IMU_KD, 255);
int targetHeading = 0;
Adafruit_BNO055 bno = Adafruit_BNO055(55, BNO055_ADDRESS_B, &Wire1);
//OPERATION_MODE_IMUPLUS

void setup() {
    Serial.begin(9600);
    delay(1000);

    while(!bno.begin(OPERATION_MODE_IMUPLUS)) {
        Serial.println("No BNO055 detected. Check your wiring or I2C ADDR.");
        delay(1000);
    }
    delay(500);
    bno.setExtCrystalUse(true);
    delay(500);

    gorobotgo.init();
    out.init();
    orbit.init();
  
  


}


// void loop() {
//     sensors_event_t compass;
//     bno.getEvent(&compass);
//     // float heading = float(compass.orientation.x);
//     Serial.print(compass.orientation.x);
//     // if (heading > 180.0f) {
//     //     heading -= 360.0f;
//     // }

//     // float dir = orbit.orbit();
//     // float rotation = -correction.update(heading, targetHeading);

//     // gorobotgo.move(0.0, rotation, 0.0);
//     // Serial.print("correction:");
//     // Serial.print(rotation);
//     Serial.println();


    
//     // digitalWrite(INA_1, HIGH);
//     // digitalWrite(INB_1, LOW);
//     // analogWrite(PWM_1, 25);

//     // digitalWrite(INA_2, HIGH);
//     // digitalWrite(INB_2, LOW);
//     // analogWrite(PWM_2, 25);

//     // digitalWrite(INA_3, HIGH);
//     // digitalWrite(INB_3, LOW);
//     // analogWrite(PWM_3, 25);

//     // digitalWrite(INA_4, HIGH);
//     // digitalWrite(INB_4, LOW);
//     // analogWrite(PWM_4, 25);
//     //seymore sucks
//     //testtesttest
// }  

void loop() {
    static uint32_t last = 0;
    if (millis() - last < 10) return;
    last = millis();

    sensors_event_t compass;
    bno.getEvent(&compass);

    uint8_t sys, gyro, accel, mag;
    bno.getCalibration(&sys, &gyro, &accel, &mag);

    uint8_t sysStat, selfTest, sysErr;
    bno.getSystemStatus(&sysStat, &selfTest, &sysErr);

    Serial.printf("t=%lu  hdg=%.2f  gyroCal=%u  sysStat=%u  sysErr=%u  mode=%u\n",
                  millis(), compass.orientation.x, gyro, sysStat, sysErr,
                  bno.getMode());
}