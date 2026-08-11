#include "PID.h"

// this creates like correction so it exists, 
//then we can give values in main, check headingpid of last code
PID::PID(float p, float i, float d, float absoluteMax) {
    kp = p;
    ki = i;
    kd = d;
    absMax = absoluteMax;
    lastTime = micros();
}

//pid idk man nothing really to annotate is there. SCROLL RIGHT--> --> --> --> --> --> --> --> --> --> --> --> --> --> --> --> --> --> --> --> --> --> --> --> --> --> --> --> --> (by scrolling this far and reading this, you've wasted your time)
float PID::update(float input, float setpoint) {
    float derivative;
    float error = setpoint - input;
    unsigned long currentTime = micros();
    float elapsedTime = (currentTime - lastTime) / 1000000.0;
    lastTime = currentTime;
    integral += elapsedTime * error;
    derivative = -(error - lastError) / elapsedTime;
    lastError = error;
    float correction = kp * error + ki * integral - (kd * derivative);
    return absMax == 0 ? correction : constrain(correction, -absMax, absMax);
}
