#include <motors.h>

void Motors::init(){
    for (uint8_t i = 0; i < 4; i++) {
        pinMode(pwm[i], OUTPUT);
        pinMode(inA[i], OUTPUT);
        pinMode(inB[i], OUTPUT);
   }
}

//calculate movement direction and set motors to move based off direction.
void Motors::move(float direction, float correction, float speed){
    float speeds[4];
    for (uint8_t i = 0; i < 4; i++) {
        speeds[i] = speed * cos((45.0 - i * 90.0 + direction) * DEG_TO_RAD) + correction;  
    }
    //if highest goes over max (100) scale everything down by whatever ratio
    float highest_value = max(max(max(abs(speeds[0]), abs(speeds[1])), abs(speeds[2])), abs(speeds[3]));
    if (highest_value > 100.0){
        for (uint8_t i = 0; i < 4; i++){
            speeds[i] *= 100.0 / highest_value;
        }
    }
    //assign speeds to motors
    for (uint8_t i = 0; i < 4; i++) {
        digitalWrite(inA[i], speeds[i] > 0.0);
        digitalWrite(inB[i], speeds[i] < 0.0);
        analogWrite(pwm[i], round(2.0 * abs(speeds[i])));
    }  
}