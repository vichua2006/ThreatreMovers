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
    this->new_starting_address(start_chan);
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

/// @brief update the starting dmx channel/address
/// @param address the new starting address
void MovingHead:: new_starting_address(int address){

    START_CHANNEL = address;

    // update all other channel positions
    MASTER_DIMMER = address + RelativeMasterDimmer;
    RED_CHAN = address + RelativeRedChan;
    GREEN_CHAN = address + RelativeGreenChan;
    BLUE_CHAN = address + RelativeBlueChan;
    PAN_CHAN = address + RelativePanChan;
    TILT_CHAN = address + RelativeTiltChan;
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

    // if the last packet of dmx data was more than 5 seconds ago 
    if (DMXSerial.noDataSince() < (unsigned long) 5000){
        // int panPos = map(read_dmx_channel(PAN_CHAN), 0, 255, PanInputLower, PanInputUpper);
        // int tiltPos = map(read_dmx_channel(TILT_CHAN), 0, 255, TiltInputLower, TiltInputUpper);

        int panPos = (double) read_dmx_channel(PAN_CHAN) / 255.0 * PanInputUpper;
        int tiltPos = (double) read_dmx_channel(TILT_CHAN) / 255.0 * TiltInputUpper;

        this->move_towards(panPos, tiltPos);
    } else {
        this->move_towards(0, 0);
    }
}

int MovingHead:: read_dmx_channel(int channel){
    return DMXSerial.read(channel);
}

#endif