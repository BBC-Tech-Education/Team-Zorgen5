// #include <motors.h>
// #include <Light Sensors.h>
// #include <pins.h>
// #include <Orbit.h>
// #include <constants.h>
// #include <Adafruit_BNO055.h>
// #include <Adafruit_I2CDevice.h>
// #include <Adafruit_Sensor.h>
// #include <PID.h>
// #include <Arduino.h>
// #include "Common.h"

// Motors gorobotgo;
// LightSensors ls;
// Orbit orbit;
// PID correction(IMU_KP, IMU_KI, IMU_KD, 255);
// Adafruit_BNO055 bno = Adafruit_BNO055(55, BNO055_ADDRESS_B, &Wire1);

// float heading = 0.0f;
// float line_direction = -1.0f;
// float line_status = 0.0f;



// void update_line_direction()
// {
//     float raw_line_dir = ls.get_line_direction();

//     float adjusted_line_dir = raw_line_dir == -1.0f ? -1.0f : float_mod(raw_line_dir + heading, 360.0f);

//     if (line_status == 0.0f) {
//         if (adjusted_line_dir != -1.0f) {
//             line_status = 1.0f;
//             line_direction = adjusted_line_dir;
//         }
//     } else if (line_status == 1.0f) {
//         if (adjusted_line_dir == -1.0f) {
//             line_direction = -1.0f;
//             line_status = 0.0f;
//         } else if (smallest_angle_between(adjusted_line_dir, line_direction) <= 90.0f) {
//             line_direction = adjusted_line_dir;
//         } else {
//             line_direction = float_mod(adjusted_line_dir + 180.0f, 360.0f);
//             line_status = 2.0f;
//         }
//     } else if (line_status == 2.0f) {
//         if (adjusted_line_dir == -1.0f) {
//             line_status = 3.0f;
//         } else if (smallest_angle_between(adjusted_line_dir, line_direction) <= 90.0f) {
//             line_direction = adjusted_line_dir;
//             line_status = 1.0f;
//         } else {
//             line_direction = float_mod(adjusted_line_dir + 180.0f, 360.0f);
//         }
//     } else {
//         if (adjusted_line_dir != -1.0f) {
//             line_direction = float_mod(adjusted_line_dir + 180.0f, 360.0f);
//             line_status = 2.0f;
//         }
//     }
// }


// void setup() {
//     delay(1000);

//     while(!bno.begin(OPERATION_MODE_CONFIG)) {
//         Serial.println("No BNO055 detected. Check your wiring or I2C ADDR.");
//         delay(1000);
//     }
//     delay(25);
//     bno.setAxisRemap(Adafruit_BNO055::REMAP_CONFIG_P1);
//     bno.setAxisSign(Adafruit_BNO055::REMAP_SIGN_P7);
//     delay(25);
//     bno.setMode(OPERATION_MODE_IMUPLUS);
//     delay(25);


//     gorobotgo.init();
//     ls.init();
//     orbit.init();
// }

// void loop()
// {
//     sensors_event_t compass;
//     bno.getEvent(&compass);

//     heading = compass.orientation.x;

//     float rotation = -correction.update((heading > 180.0f) ? heading - 360.0f : heading, 0);

//     ls.update();
//     update_line_direction();
//     Serial.println();
//     Serial.println(ls.get_line_direction());
    
//     // Serial.println(line_direction);
//     float movedir;
//     float movespeed;

//     if (line_direction != -1.0f) {
//         movedir = float_mod(line_direction + 180.0f, 360.0f);
//         movespeed = 200;
//     } else {
//         movedir = orbit.orbit();

//         if (movedir == -0.6767) {
//             movespeed = 0.0f;
//         } else {
//             movespeed = 100.0f;
//         }
//     }
//     // Serial.println(movedir);

//     gorobotgo.move(movedir, rotation, movespeed);
// }


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
        gorobotgo.move(github, -rotation, 70.0f);
    } else if (movedir == -0.6767) {
        gorobotgo.move(0.0f, -rotation, 0.0f);
    } else if (movedir > -1 && movedir < 1) {
        gorobotgo.move(0.0f, -rotation, 20.0f);
    } else {
        gorobotgo.move(movedir, -rotation, 20.0f);
    }

}
