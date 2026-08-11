#include <motorPID.h>
#include <Orbit.h>

motorPID::motorPID(float p, float i, float d, float absoluteMax) {
    kp = p;
    ki = i;
    kd = d;
    absMax = absoluteMax;
    lastTime = micros();
}

//different pid meant for if i wanted to tweak individually how the pid worked or the p i or d values.
float motorPID::update(float input, float setpoint) {
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