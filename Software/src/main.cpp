#include <motors.h>
#include <Light Sensors.h>
#include <pins.h>
#include <Orbit.h>
#include <constants.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_Sensor.h>
#include <PID.h>
#include <Arduino.h>

Motors gorobotgo;
LightSensors ls;
Orbit orbit;
PID correction(IMU_KP, IMU_KI, IMU_KD, 255);
int targetHeading = 0;
Adafruit_BNO055 bno = Adafruit_BNO055(55, BNO055_ADDRESS_B, &Wire1);
// OPERATION_MODE_IMUPLUS

void setup() {
    Serial.begin(9600);
    delay(1000);

    while(!bno.begin(OPERATION_MODE_CONFIG)) {
        Serial.println("No BNO055 detected. Check your wiring or I2C ADDR.");
        delay(1000);
    }
    delay(25);

    bno.setAxisRemap(Adafruit_BNO055::REMAP_CONFIG_P1);
    bno.setAxisSign(Adafruit_BNO055::REMAP_SIGN_P7);
    delay(25);

    bno.setMode(OPERATION_MODE_IMUPLUS);
    delay(25);


    gorobotgo.init();
    ls.init();
    orbit.init();
    pinMode(32,OUTPUT);
    pinMode(31,OUTPUT);
    pinMode(28,OUTPUT);
    analogWriteFrequency(28,15000);
    
}

  

void loop() {

    static uint32_t last = 0, lastStatus = 0;
    static uint8_t sysStat = 0, selfTest = 0, sysErr = 0;

    if (millis() - last < 10) return;
    last = millis();

    sensors_event_t compass;
    bno.getEvent(&compass);

    uint8_t sys, gyro, accel, mag;
    bno.getCalibration(&sys, &gyro, &accel, &mag);

    if (millis() - lastStatus > 2000) {
        lastStatus = millis();
        bno.getSystemStatus(&sysStat, &selfTest, &sysErr);
    }

    Serial.printf("t=%lu  hdg=%.2f  gyroCal=%u  mode=%u  sysStat=%u  sysErr=%u   %u\n",
                  millis(), compass.orientation.x, gyro,
                  bno.getMode(), sysStat, sysErr, sys);

    
    float heading = float(compass.orientation.x);
    // Serial.print(compass.orientation.x);
    if (heading > 180.0f) {
        heading -= 360.0f;
    }
    // Serial.println(heading);
    float rotation = correction.update(heading, targetHeading);
    // ls.read();
    Serial.println(rotation);
    // gorobotgo.move(0.0f, -rotan  tion, 0.0f);
    // orbit.orbit();
    
    // ls.read();

    // gorobotgo.test();
 
}
