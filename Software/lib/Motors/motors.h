#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>
#include <pins.h>

class Motors{
public: 
    Motors() {};
    void init();
    void move(float direction, float correction, float speed);
private:
    uint8_t pwm[4] = {PWM_1, PWM_2, PWM_3, PWM_4};
    uint8_t inA[4] = {INA_1,INA_2,INA_3,INA_4};
    uint8_t inB[4] = {INB_1,INB_2,INB_3,INB_4};

};

#endif