#ifndef ORBIT_H
#define ORBIT_H

#include <Arduino.h>
#include <pins.h>

class Orbit{

public:
    Orbit(){};
    float orbit();
    void init();
    bool ball();
    float distance();
private:
    int sensor_values [TSSP_NO];
    int IR_pins [TSSP_NO] = {
        IR_0 ,
        IR_1 ,
        IR_2 ,
        // IR_3 ,
        // IR_4 ,
        // IR_5 ,
        // IR_6 ,
        // IR_7 ,
        // IR_8 ,
        // IR_9 ,
        // IR_10 ,
        // IR_11
    };
};




#endif