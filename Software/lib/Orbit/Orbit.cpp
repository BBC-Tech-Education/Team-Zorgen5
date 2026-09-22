#include <Orbit.h>


void Orbit::init(){
    for (int i = 0; i < TSSP_NO; i++){
        pinMode(IR_pins[i], INPUT);
    }
}

//calculate ball direction
float Orbit::orbit() {
    for (int i = 0; i < TSSP_NO; i++){
        sensor_values[i] = 0;
    }
    for(int i = 0; i < 100; i++) {
        for(int t = 0; t < TSSP_NO; t++) {
            sensor_values[t] += 1 - digitalRead(IR_pins[t]);
        }
        delayMicroseconds(10);
    }
    for (int j = 0; j < TSSP_NO; j++) {
        Serial.print(sensor_values[j]);
        Serial.print(" ");
    }
    Serial.println("");
    // for (int t = 0; t < TSSP_NO; t++) {
    //     sensor_values[t] = 255 * (analogRead(IR_pins[t]) / 1023);
    // }
    // for (int i = 0; i < 3; i++){
    //     if (sensor_values[i] == 255){
    //         sensor_values[i] = (sensor_values[i-1] + sensor_values[i+1])/2;
    //     }
    // }
    int last_highest = 0;
    int current_highest = 0;
    int direction = 0;
    for (int i = 0; i < TSSP_NO; i++){
        current_highest = max(sensor_values[i], last_highest);
        if (current_highest > last_highest) {
            direction = i;
        }
        last_highest = current_highest;
    }
    // for (int i = 0; i < TSSP_NO; i++){
    //     Serial.print(sensor_values[i]);
    //     Serial.print (" ");
    // }
    // Serial.print(direction);
    // Serial.println();
    // direction *= (360/TSSP_NO);

    
    //calculate coordinates
    // float x_coord = 0;
    // x_coord = current_highest * sin(direction);
    // float y_coord = 0;
    // y_coord = current_highest * cos(direction);
    // float x_target = 0;
    // float y_target = y_coord - 100;
    
    // PID.update(x_coord, x_target);

    return direction; 
}

float Orbit::distance() {
    for (int i = 0; i < TSSP_NO; i++){
        sensor_values[i] = 0;
    }
    for(int i = 0; i < 255; i++) {
        for(int t = 0; t < TSSP_NO; t++) {
            sensor_values[t] += 1 - digitalRead(IR_pins[t]);
        }
    }
    for (int i = 0; i < 12; i++){
        if (sensor_values[i] == 255){
            sensor_values[i] = (sensor_values[i-1] + sensor_values[i+1])/2;
        }
    }
    int last_highest = 0;
    int current_highest = 0;
    for (int i = 0; i < TSSP_NO; i++){
        current_highest = max(sensor_values[i], last_highest);
        last_highest = current_highest;
    }
    return current_highest;
}





// if (direction < 90) {
    //     x_coord = current_highest * sin(direction);
    // }
    // // else if (direction > 90 && direction < 180) {
    // //     x_coord = current_highest * sin(direction);
    // // }
    // // else if (direction < 0 && direction > -90) {
    // //     x_coord = current_highest * sin(direction) * -1;
    // // }
    // // else if (direction < -90 && direction > -180) {
    // //     x_coord = current_highest * sin(direction);
    // // }
    // if (direction < 90) {
    //     y_coord = current_highest * cos(direction);
    // }
    // else if (direction > 90 && direction < 180) {
    //     y_coord = current_highest * cos(direction);
    // }
    // else if (direction < 0 && direction > -90) {
    //     y_coord = current_highest * cos(direction) * -1;
    // }
    // else if (direction < -90 && direction > -180) {
    //     y_coord = current_highest * cos(direction);
    // }
