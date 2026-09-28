#include "Light Sensors.h"
#include <Arduino.h>
#include "constants.h"


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

        value[muxorder[i]] = analogRead(LS_OUT0);
        value[muxorder[i+16]] = analogRead(LS_OUT1);


        // Serial.print(analogRead(LS_OUT0));
        // Serial.print(" ");
    }
    // Serial.println("           0               0          ");

    

    // Serial.println(analogRead(LS_OUT1));
    // delay(250);
}

void LightSensors::setgreen(){
    for (int i = 0; i < 32; i++) {
        green[i] += value[i];
        green[i] /= 10;
        green[i] += LS_BUFFER;
        Serial.print(green[i]);
        Serial.print ("   ");
    }

}



//     for (int i = 0; i < 10; i++) {
//         for (int j = 0; j < LIGHT_NO; j++) {
//             green[j] += analogRead(pin[j]);
//         }
//     }

// }


// detect line
// int LightSensors::lineDetection(){
//     read();
//     int light_total = 0;
//     int white_sensors = 0;
//             // Serial.print(white_sensors);
//             // Serial.print(light_total);
//             // Serial.print("here");
//     for (int i = 0; i < LS_DIGITAL_NUM; i++){
//         if(onWhite[i] == true && onWhite[16 - i] == true){
//             light_total += 0;
//             white_sensors += 2;

// detect line
float LightSensors::lineDetection(){
    read();
    float light_total = 0;
    int white_sensors = 0;
            // Serial.print(white_sensors);
            // Serial.print(light_total);
            // Serial.print("here");

    for (int j = 0; j < 32; j++) {
        onWhite[j] = false;
    }

    for (int i = 0; i < 32; i++){
        if (value[i] > green[i]){
            onWhite[i] = true;
        } else {
            onWhite[i] = false;
        }
        if (onWhite[i] == true && onWhite[31 - i] == true) {
            light_total += 0;
            white_sensors += 2;
        }
        else if (onWhite[i] == true){            
            light_total += i * 11.25; //(360/32)
            white_sensors++;
        }

        // Serial.print(onWhite[i]);
        // Serial.print("    ");
    }

    // if (onWhite[0] == true || onWhite[1] == true || onWhite[2] == true || onWhite[3] == true ){
    //     bool flip = true;
    //     for (int i = 0; i < 16; i++){
    //         if (onWhite[i] == true){            
    //         light_total += i * 11.25; //(360/32)
    //         white_sensors++;
    //         }
    //     }
    //     for (int i = 16; i < 32; i++){
    //         if (onWhite[i] == true){
    //         light_total -= (32-i) * 11.25; //(360/32)
    //         white_sensors++;
    //         }
    //     }
    // }
    // else{ 
    //     bool flip = false;
    //     for(int i = 0; i < 32; i++){
    //     if (onWhite[i] == true){
    //     light_total += i * 11.25; //(360/32)
    //     white_sensors++;
    //     }    
    // }




//  else if (onWhite[i] == true) {
//         light_total += i * 11.25; //(360/32)
//         white_sensors++;
//     }    


    // }
    // Serial.println("");
    // Serial.print(light_total);
    if (white_sensors == 0){
        float line_direction = -1.0f;
        return line_direction;
    } else if (white_sensors == 32) {
        float line_direction = -1.0f;
        return line_direction;
    } else {
        float line_direction = (light_total / white_sensors);
        return line_direction;
    }

}

//remembrance for if fully crossed line
int LightSensors::lineRemembrance(){
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
        if (line_dir == -1) {
        current_status = 0;
        } else if (difference > 90) {
            current_status = 2;
        } else {
            current_status = 1;
        }
    } else if (current_status == 2) {
        if (line_dir == -1) {
        current_status = 3;
        } else if (difference > 90) {
            current_status = 1;
        } else {
            current_status = 2;
        }
    } else if (current_status == 3) {
        if (line_dir /= -1) {
            current_status = 2;
        } else {
        current_status = 3;
        }
    }

    Serial.print("status: ");
    Serial.println(current_status);
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
        move_dir = 0; /// fix 
    }
    return move_dir;
}



// */