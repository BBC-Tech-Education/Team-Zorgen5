#ifndef LIGHT_SENSORS_H
#define LIGHT_SENSORS_H


#include <pins.h>
#include <stdint.h>

#define LS_NUM 32
#define LS_DIGITAL_NUM 4   


class LightSensors {
public:
    LightSensors() {};

    void init();
    void update();
    float calculate_line_direction();
    float calculate_line_rememberance();

private:
    void read();
    void calibrate();
    // float cluster2 = 0;

    float line_direction;

    float test_status = 0;

    float float_mod(float x, float y);
    float angle_between(float angle_left, float angle_right);
    float smallest_angle_between(float angle_left, float angle_right);
    float mid_angle_between(float angle_left, float angle_right);





    int current_status = 0;

    int facing_before = 0;
    int last_known = 0;
    uint16_t green[LS_NUM] = {0};
    uint16_t value[LS_NUM] = {0};
    uint8_t digital_pin[LS_DIGITAL_NUM]={LS_D0, LS_D1, LS_D2, LS_D3};
    uint8_t muxorder[32] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,28,27,26,25,24,23,22,21,32,31,30,29,17,18,19,20};
};


#endif // LIGHT_SENSORS_H