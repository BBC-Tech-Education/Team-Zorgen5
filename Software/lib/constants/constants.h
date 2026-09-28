#ifndef CONSTANTS_H
#define COSTANTS_H

#include <Arduino.h>

#define IMU_KP 0.6
#define IMU_KI 0.01
#define IMU_KD 0.043

#define MOTOR_KP -0.35
#define MOTOR_KI -0.1
#define MOTOR_KD -0.05

#define OrbitP 254.99902
#define OrbitQ 0.00630627
#define OrbitR 1.57207

#define OrbitA 0.00000928084
#define OrbitB 0.0001443
#define OrbitC 1.59543
#define OrbitD 0.909091


#define LS_BUFFER 200

#define Vision Serial1
#define Bytesrequiredforpacket 6

#endif