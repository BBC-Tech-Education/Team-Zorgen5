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
    
    calibrate();
}



void LightSensors::read()
{
    for (uint8_t i = 0; i < 16; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            digitalWriteFast(digital_pin[j], (i >> j) & 1);
        }
        delayMicroseconds(10);

        value[muxorder[i]] = analogRead(LS_OUT0);
        value[muxorder[i+16]] = analogRead(LS_OUT1);
    }
}

void LightSensors::calibrate()
{
    for (uint8_t i = 0; i < LS_NUM; i++) {
        green[i] = 0;
    }

    for (uint8_t i = 0; i < 10; i++) {
        read();
        for (uint8_t j = 0; j < LS_NUM; j++) {
            green[j] += value[j];
        }
    }

    for (uint8_t i = 0; i < LS_NUM; i++) {
        green[i] /= 10;
        green[i] += LS_BUFFER;
    }
}



void LightSensors::update()
{
    read();

    calculate_line_direction();
    calculate_line_rememberance();
}

float LightSensors::calculate_line_direction()
{
    uint8_t onWhite[LS_NUM] = {0};

    for (uint8_t i = 0; i < LS_NUM; i++) {
        onWhite[i] = (value[i] > green[i]);
        // Serial.print(value[i]); Serial.print(" ");
    }
    Serial.println();

    uint8_t cluster_start[4] = {0};
    uint8_t cluster_end[4]   = {0};
    uint8_t cluster_num      = 0;

    uint8_t in_cluster = 0;

    for (uint8_t i = 0; i < LS_NUM; i++) {
        if (in_cluster) {
            if (!onWhite[i]) {
                in_cluster = 0;
                cluster_end[cluster_num] = i - 1;
                cluster_num++;
            }
        } else {
            if (onWhite[i]) {
                in_cluster = 1;
                cluster_start[cluster_num] = i;
            }
        }
    }


    if (onWhite[0] == false && onWhite[31] == true) {
        cluster_num += 1;
    }
    // if (onWhite[0] && onWhite[LS_NUM - 1]) {
    //     cluster_start[0] = cluster_start[cluster_num];
    // }


    if (cluster_num == 0) {
        line_direction = -1;

    } else if (cluster_num == 1) {
        // Serial.println("gold digger");
        line_direction = mid_angle_between(cluster_start[0] * 11.25f, cluster_end[0] * 11.25f);
    } else if (cluster_num == 2) {
        // Serial.println("chudkys");
        float cluster1 = mid_angle_between(cluster_start[0] * 11.25f, cluster_end[0] * 11.25f);
        float cluster2 = mid_angle_between(cluster_start[1] * 11.25f, cluster_end[1] * 11.25f);
        // Serial.println(cluster1);
        // Serial.println(cluster2);
        float angle = angle_between(cluster1, cluster2);
        if (angle < 180){
            // Serial.print("kok");
            line_direction = mid_angle_between(cluster1, cluster2);
        } else { 
            // Serial.print("azz");
            line_direction = mid_angle_between(cluster2, cluster1);
        }



    }
    // Serial.print(cluster_start[1] * 11.25);
    // Serial.print(" ");
    // Serial.println(cluster_end[1] * 11.25);
    // Serial.println(cluster_num);
    return line_direction;
}


float LightSensors::calculate_line_rememberance() {
    int line_dir = calculate_line_direction();
    Serial.println(line_dir);
    // delay(25);
    int difference = fmin(abs(line_dir - first_touch),(360-abs(line_dir - first_touch)));
    int move_dir = 0;
    if (current_status == 0) {
        if (line_dir > 0) {
            current_status = 1;
            first_touch = line_dir;
        } else {
            current_status = 0;
        }
    } else if (current_status == 2) {
        if (line_dir == -1) {
            current_status = 3;
        } else if (difference < 100) {
            current_status = 1;
        } else {
            current_status = 2;
            last_known = line_dir;
        }
    } else if (current_status == 3) {
        if (line_dir != -1) {
            current_status = 2;
        } else {
            current_status = 3;
        }
    } else if (current_status == 1) {
        if (line_dir != -1) {
            if (difference > 100) {
                current_status = 2;
                last_known = line_dir;
            }
        } else if (line_dir == -1) {
            current_status = 0;
        } else {
            current_status = 1;
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
        move_dir = last_known;
        Serial.print("lastknown:");
        Serial.println(last_known);
    }
    return move_dir;
}




float LightSensors::float_mod(float x, float y) {
    float r = fmod(x,y);
    if (r < 0){
        return r + y;
    } else{
        return r;
    }
}

// setup for clusters
float LightSensors::angle_between(float angle_left, float angle_right)
{   
    float albert = float_mod(abs(angle_left - angle_right), 360.0f);
    return albert;
}

float LightSensors::smallest_angle_between(float angle_left, float angle_right)
{
    float angle = angle_between(angle_left, angle_right);
    return fmin(angle, 360.0f - angle);
}

float LightSensors::mid_angle_between(float angle_left, float angle_right)
{
    if (angle_between(angle_left, angle_right) > 180){
        float mucus = float_mod(angle_left + 0.5f * smallest_angle_between(angle_left , angle_right), 360.0f);
        // Serial.print("poo");
        return mucus;
    } else {
        float mucus = float_mod(angle_left + 0.5f * angle_between(angle_left , angle_right), 360.0f);
        return mucus;
    }
    // Serial.println (mucus);

}
// float LightSensors::smallest_mid_angle_between(float angle_left, float angle_right)
// {
//     return float_mod(angle)
// }


/*
        if (onWhite[i] == true && onWhite[31 - i] == true) {
            light_total += 0;
            white_sensors += 2;
        }
        else if (onWhite[i] == true){            
            light_total += i * 11.25; //(360/32)
            white_sensors++;
        }

        // Serial.print(onWhite[i]);
        // Serial.print(" ");
    }
    delay(250);
    if (onWhite[0] == true || onWhite[1] == true || onWhite[31] == true) {
        if (onWhite[9] == false && onWhite[24] == false){
            return 0;
        }
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
    // delay(500);
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
            last_known = facing_before;
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
        if (line_dir != -1) {
            current_status = 2;
        } else {
        current_status = 3;
        }
    }

    // Serial.print("status: ");
    // Serial.println(current_status);
    //based off current status (in or out or on line), move in differetn direction.
    if (current_status == 0 || current_status == 1 || current_status == 2) {
        facing_before = line_dir;
        if (current_status == 0) {
        move_dir = 0;
        } else if (current_status == 1) {
        move_dir = line_dir;
        } else if (current_status == 2) {
        move_dir = line_dir + 180;
        }
    } else {
        move_dir = last_known;
        // Serial.println(last_known);
    }
    return move_dir;
}



// */