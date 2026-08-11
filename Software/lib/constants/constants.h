#ifndef CONSTANTS_H
#define COSTANTS_H

#include <Arduino.h>

#define IMU_KP 0.35
#define IMU_KI 0
#define IMU_KD 0.05

#define MOTOR_KP -0.35
#define MOTOR_KI -0.1
#define MOTOR_KD -0.05

#define LS_BUFFER 500

#define Vision Serial1
#define Bytesrequiredforpacket 6

#endif