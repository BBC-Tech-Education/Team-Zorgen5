#include "Light Sensors.h"
#include <Arduino.h>


void LightSensors::init()
{
    for (uint8_t i = 0; i < LS_DIGITAL_NUM; i++){
        pinMode(digital_pin[i], OUTPUT);
    }
    pinMode(LS_OUT0, INPUT);
    pinMode(LS_OUT1, INPUT);
}


void LightSensors::read()
{

    // 0 ->  00000000
    // 1 ->  00000001

    // 14 -> 00001110
    //       00000001
    //       00000000

    // 14 -> 00000111
    //       00000001
    //       00000001

    // 14 -> 00000011
    //       00000001
    //       00000001

    // 14 -> 00000001
    //       00000001
    //       00000001

    for (uint8_t i = 0; i < 16; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            digitalWriteFast(digital_pin[j], (i >> j) & 1);
        }
        delayMicroseconds(10);

        value[i] = analogRead(LS_OUT0);
        value[i] = analogRead(LS_OUT1);

        // Serial.print(analogRead(LS_OUT0));
        // Serial.print(" ");
    }
    // Serial.println();

    

    // Serial.println(analogRead(LS_OUT1));
    // delay(250);
}


/*




    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < LIGHT_NO; j++) {
            green[j] += analogRead(pin[j]);
        }
    }
    for (int i = 0; i < LIGHT_NO; i++) {
        green[i] /= 10;
        green[i] += LS_BUFFER;
    }
}

//read sensors
void Out::read() {
    for (int i = 0; i < LIGHT_NO; i++) {
        value[i] = analogRead(pin[i]);
        Serial.print(value[i]);
        Serial.print(" ");
        onWhite[i] = value[i] > green[i];
        // Serial.print(onWhite[i]);
    }
    Serial.println();
}

//detect line
int Out::lineDetection(){
    read();
    int light_total = 0;
    int white_sensors = 0;
            // Serial.print(white_sensors);
            // Serial.print(light_total);
            // Serial.print("here");
    for (int i = 0; i < LIGHT_NO; i++){
        if(onWhite[i] == true && onWhite[16 - i] == true){
            light_total += 0;
            white_sensors += 2;

        }
        else if (onWhite[i] == true){
            light_total += i * 360 / LIGHT_NO;
            white_sensors++;
        }    
    }
    if (white_sensors == 0){
        int line_direction = -1;
        return line_direction;
    }
    int line_direction = light_total / white_sensors;
    return line_direction;
    // Serial.print(line_direction);
}

//remembrance for if fully crossed line
int Out::lineRemembrance(int current_dir){
    int line_dir = lineDetection();
    int difference = abs(line_dir - facing_before);
    int move_dir = 0;
    if (current_status == 0) {
        if (line_dir > 0) {
            current_status = 1;
        } else {
        current_status = 0;
        }
    } else if (current_status == 1) {
        if (difference > 150) {
        current_status = 2;
        } else if (line_dir == 0) {
            current_status = 0;
        } else {
            current_status = 1;
        }
    } else if (current_status == 2) {
        if (difference > 150) {
        current_status = 1;
        } else if (line_dir == 0) {
            current_status = 3;
        } else {
            current_status = 1;
        }
    } else if (current_status == 3) {
        if (line_dir > 0) {
            current_status = 2;
        } else {
        current_status = 3;
        }
    }

    //based off current status (in or out or on line), move in differetn direction.
    if (current_status == 0 || current_status == 1 || current_status == 2) {
        facing_before = line_dir;
        if (current_status == 0) {
        move_dir = 0;
        } else if (current_status == 1) {
        move_dir = line_dir + 180;
        } else if (current_status == 2) {
        move_dir = line_dir;
        }
    } else {
        move_dir = facing_before;
    }
    return move_dir;
}



*/