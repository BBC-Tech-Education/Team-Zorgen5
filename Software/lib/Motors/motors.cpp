#include <motors.h>

void Motors::init(){
   for (uint8_t i = 0; i < 4; i++) {
        pinMode(inA[i], OUTPUT);
        pinMode(inB[i], OUTPUT);
        pinMode(pwm[i], OUTPUT);
    
        // Starts up motor controllers properly to avoid going into latch state
        digitalWrite(inA[i], HIGH);
        digitalWrite(inB[i], HIGH);
        delayMicroseconds(100);
        analogWriteFrequency(pwm[i], 15000); // 15000 less than recommended maximum of 20000 from datasheet
        analogWrite(pwm[i], 0);
   }
}

//calculate movement direction and set motors to move based off direction.
void Motors::move(float direction, float correction, float speed){
    float speeds[4];
    for (uint8_t i = 0; i < 4; i++) {
        speeds[i] = speed * cos((45.0 - i * 90.0 + direction) * DEG_TO_RAD) - correction; 
    }
    //if highest goes over max (100) scale everything down by whatever ratio
    float highest_value = max(max(max(fabs(speeds[0]), fabs(speeds[1])), fabs(speeds[2])), fabs(speeds[3]));
    if (highest_value > 100.0){
        for (uint8_t i = 0; i < 4; i++){
            speeds[i] *= 100.0 / highest_value;
        }
    }
    //assign speeds to motors
    for (uint8_t i = 0; i < 4; i++) {
        digitalWrite(inA[i], speeds[i] > 0.0);
        digitalWrite(inB[i], speeds[i] < 0.0);
        analogWrite(pwm[i], round(2.0 * fabs(speeds[i])));
    }  
}

void Motors::test(){
    for (uint8_t i = 0 ; i < 4 ; i++){
        digitalWrite(inA[i], HIGH);
        digitalWrite(inB[i], LOW);
        analogWrite(pwm[i], 200);
        delay(1000);
        digitalWrite(inA[i], LOW);
        digitalWrite(inB[i], HIGH);
        analogWrite(pwm[i], 200);
        delay(1000);
    }

}

