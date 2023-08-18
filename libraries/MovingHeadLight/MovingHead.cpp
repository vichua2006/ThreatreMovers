/* source file for moving head light object
 * Created by: Victor Huang and Rohan Katreddy
 * June 22, 2023
 */

#include "MovingHead.h"
#include <Arduino.h>
#include <config.h>


#ifdef USING_DMX
#include <DMXSerial.h>
#endif

MovingHead:: MovingHead(DualStepper &steppers, DualHallSensors &sensors, int start_chan) : panTilt(steppers), panTiltHallSensor(sensors){
    // reference members must be already initalized
    this->new_start_channel(start_chan);
}

void MovingHead:: init_pin_mode(){
    panTilt.init_pin_mode();
    panTiltHallSensor.init_pin_mode();

    #ifdef USING_DMX
    DMXSerial.init(DMXReceiver, DMXReceiverPin);
    #endif
}

void MovingHead:: move_towards(double pan_pos, double tilt_pos){
    panTilt.step_towards(pan_pos, tilt_pos);
}

void MovingHead:: find_home(){
    panTilt.home(panTiltHallSensor);
}

void MovingHead:: new_start_channel(int new_channel){

    START_CHANNEL = new_channel;

    // update all other channel positions
    MASTER_DIMMER = new_channel + RelativeMasterDimmer;
    RED_CHAN = new_channel + RelativeRedChan;
    GREEN_CHAN = new_channel + RelativeGreenChan;
    BLUE_CHAN = new_channel + RelativeBlueChan;
    PAN_CHAN = new_channel + RelativePanChan;
    TILT_CHAN = new_channel + RelativeTiltChan;
}


/// @brief set the maximum speeds that pan/tilt will accelerate to during movement, in steps per second
/// @param pan_speed max speed of pan, 
/// @param tilt_speed max speed of tilt
void MovingHead:: set_speed(int pan_speed, int tilt_speed){
    panTilt.set_max_speed(pan_speed, tilt_speed);
}

/// @brief Set the rate of acceleration of pan and tilt stepper motors, in steps per second per second. Rate must be > 0.0. Don't call more often than neccesary.
/// @param pan_accel acceleration of pan
/// @param tilt_accel acceleration of tilt
void MovingHead:: set_accel(int pan_accel, int tilt_accel){
    panTilt.set_acceleration(pan_accel, tilt_accel);
}

#ifdef USING_DMX

void MovingHead:: main_cycle(){

    int panPos = map(read_dmx_channel(PAN_CHAN), 0, 255, PanInputLower, PanInputUpper);
    int tiltPos = map(read_dmx_channel(TILT_CHAN), 0, 255, TiltInputLower, TiltInputUpper);

    this->move_towards(panPos, tiltPos);
}

int MovingHead:: read_dmx_channel(int channel){
    return DMXSerial.read(channel);
}

#endif