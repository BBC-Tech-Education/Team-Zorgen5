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


void setup() {
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
}

  

void loop()
{
    sensors_event_t compass;
    bno.getEvent(&compass);

    float heading = compass.orientation.x;
    if (heading > 180.0f) {
        heading -= 360.0f;
    }
    // Serial.println(heading);
    float rotation = correction.update(heading, 0);
    // Serial.println(rotation);

    ls.update();
    

    float movedir = orbit.orbit();
    ls.update();
    float detect = ls.calculate_line_direction();
    float github = ls.calculate_line_rememberance();
    // Serial.println(github);
    // gorobotgo.move(0.0f, -rotation, 0.0f);
    // gorobotgo.move(movedir + 180, -rotation, 25.0f);
// <<<<</<< HEAD
    // Serial.println(pleaseeeee + 180);
    Serial.println(movedir);
    if (detect != -1) {
        gorobotgo.move(github, -rotation, 100.0f);
    } else if (movedir == -0.6767) {
        gorobotgo.move(0.0f, -rotation, 0.0f);
    } else if (movedir > -1 && movedir < 1) {
        gorobotgo.move(0.0f, -rotation, 30.0f);
    } else {
        gorobotgo.move(movedir, -rotation, 27.0f);
    }
// =======
// <<<<<<< Updated upstream
    // orbit.orbit();
//     Serial.println(movedir);
//     if (detect == -1) {
//         gorobotgo.move(movedir, -rotation, 20.0f);
//     } else {
//         gorobotgo.move(pleaseeeee + 180, -rotation, 55.0f);
//     }
// // =======
//     orbit.orbit();
    // if (detect == -1) {
    //     gorobotgo.move(movedir, -rotation, 20.0f);
    // } else {
    //     gorobotgo.move(pleaseeeee + 180, -rotation, 50.0f);
    // }
// >>>>>>> Stashed changes
// >>>>>>> 682c629ace52b932ef8864c1f3d3200e39fd20fd
    
    // ls.read();

    // gorobotgo.move(0.0f, -rotation, 0.0f);
}
