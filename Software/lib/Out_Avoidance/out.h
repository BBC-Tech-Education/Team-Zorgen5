#ifndef OUT_H
#define OUT_H

#include <Arduino.h>
#include <pins.h>
#include <constants.h>

class Out {

public:
    Out(){};
    int lineDetection();
    int lineRemembrance(int current_dir);
    void init();
private:
    void read();
    int current_status = 0;
    int facing_before = 0;
    uint16_t green[LIGHT_NO] = {0};
    uint16_t value[LIGHT_NO] = {0};
    bool onWhite[LIGHT_NO] = {false};
    uint8_t pin[LIGHT_NO]={
        LS_D0 
        LS_D1
        LS_D2 
        LS_D3
        LS_OUT0
        LS_OUT1
    };
};


#endif