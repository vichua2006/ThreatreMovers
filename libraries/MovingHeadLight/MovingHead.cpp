/* source file for moving head light object
 * Created by: Victor Huang and Rohan Katreddy
 * June 22, 2023
 */

#include "MovingHead.h"
#include <Arduino.h>
#include <DMXSerial.h>

MovingHead:: MovingHead(DualStepper &steppers, DualHallSensors &sensors) : PanTilt(steppers), PanTiltHallSensor(sensors){
    // reference members must be already initalized
}

void MovingHead:: init_pin_mode(){
    PanTilt.init_pin_mode();
    PanTiltHallSensor.init_pin_mode();
}

void MovingHead:: move_to(double pan_pos, double tilt_pos, int delay){
    PanTilt.turn_to(pan_pos, tilt_pos, delay);
}

void MovingHead:: find_home(){
    PanTilt.home(PanTiltHallSensor);
}