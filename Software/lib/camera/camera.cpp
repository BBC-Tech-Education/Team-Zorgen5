#include <camera.h>
#include <constants.h>


void Camera::init(){
    Vision.begin(115200);

}

//coordinate 0 is x and coordinate 1 is y
void Camera::interpret(){
    if (Vision.available() >= Bytesrequiredforpacket){
        //.read checks if first byte shows there is correct reading
        if (Vision.read() == 247 ){
            for (int i = 0; i < Bytesrequiredforpacket - 1; i++) {
                coordinate[i] = Vision.read();
            }
            if ((coordinate[0] < 0 && coordinate[1] > 0) || (coordinate[0] > 0 && coordinate[1] > 0)) {
                float enemy_goal = atan(coordinate[0] / coordinate[1]);
            } else if ((coordinate[0] < 0 && coordinate[1] < 0)) {
                float enemy_goal = atan(coordinate[1] / coordinate[0]) - 90;
            } else if ((coordinate[0] > 0 && coordinate[1] < 0)) {
                float enemy_goal = atan(coordinate[1] / coordinate[0]) + 90;
            }
        }
    }
}