#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>

class Camera {
public:
    Camera(){};
    void init();
    void interpret();
    float angle();
    

private:
    int coordinate[4] = {0};
};

#endif