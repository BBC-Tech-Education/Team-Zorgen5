#include "Common.h"
#include <Arduino.h>


float float_mod(float x, float y) {
    float r = fmod(x,y);
    return (r < 0) ? r + y : r;
}

float angle_between(float angle_left, float angle_right)
{   
    return float_mod(angle_right - angle_left, 360.0f);
}

float smallest_angle_between(float angle_left, float angle_right)
{
    float angle = angle_between(angle_left, angle_right);
    return fmin(angle, 360.0f - angle);
}

float mid_angle_between(float angle_left, float angle_right)
{
    return float_mod(angle_left + angle_between(angle_left, angle_right) / 2.0f, 360.0f);
}