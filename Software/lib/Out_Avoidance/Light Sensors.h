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
        void read();

        int lineDetection();
        int lineRemembrance(int current_dir);

    private:
        int current_status = 0;
        int facing_before = 0;
        uint16_t green[LS_NUM] = {0};
        uint16_t value[LS_NUM] = {0};
        uint8_t onWhite[LS_NUM] = {0};
        uint8_t digital_pin[LS_DIGITAL_NUM]={LS_D0, LS_D1, LS_D2, LS_D3};
};


#endif // LIGHT_SENSORS_H