/* source file for moving head light object
 * Created by: Victor Huang and Rohan Katreddy
 * June 22, 2023
 */

#include "MovingHead.h"
#include <Arduino.h>
#include <config.h>


#ifdef USE_DMX
#include <DMXSerial.h>
#endif

MovingHead:: MovingHead(DualStepper &steppers, DualHallSensors &sensors, int start_chan) : panTilt(steppers), panTiltHallSensor(sensors){
    // reference members must be already initalized
    new_start_channel(start_chan);
}

void MovingHead:: init_pin_mode(){
    panTilt.init_pin_mode();
    panTiltHallSensor.init_pin_mode();

    #ifdef USE_DMX
    DMXSerial.init(DMXReceiver, DMXReceiverPin);
    #endif
}

void MovingHead:: move_to(double pan_pos, double tilt_pos, int delay){
    panTilt.turn_to(pan_pos, tilt_pos, delay);
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

#ifdef USE_DMX

void MovingHead:: main_cycle(){

    int panPos = map(read_dmx_channel(PAN_CHAN), 0, 255, PanInputLower, PanInputUpper);
    int tiltPos = map(read_dmx_channel(TILT_CHAN), 0, 255, TiltInputLower, TiltInputUpper);

    move_to(panPos, tiltPos, FixedStepperDelay);
}

int MovingHead:: read_dmx_channel(int channel){
    return DMXSerial.read(channel);
}

#endif