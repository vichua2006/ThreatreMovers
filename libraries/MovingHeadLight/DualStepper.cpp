/* source file for pan tilt mechanism
 * Created by: Victor Huang and Rohan Katreddy
 * May 16, 2023
 */

#include "DualStepper.h"
#include "GeneralFunc.h"
#include "config.h"
#include <Arduino.h>


DualStepper:: DualStepper(AccelStepper &s1, AccelStepper &s2){
  steppers[0] = &s1;
  steppers[1] = &s2;
}

// enable pins for motor (cnc shield); must be at low
void DualStepper:: init_pin_mode(){
    // AccelStepper will self initalize
    
    pinMode(StepperEnablePin, OUTPUT);
    digitalWrite(StepperEnablePin, LOW);
}

// make both steppers take one step towards target position, with acceleration
// implementation follows absolutely zero oop principles and whatnot
void DualStepper:: step_towards(double ang1, double ang2){
    
    // convert degrees to number of steps, taking into account motor gear ratio and (implicitly) microsteps
    steppers[0]->moveTo((double) PanSteps * PanGR * ang1 / OneRevolution);
    steppers[1]->moveTo((double) TiltSteps * TiltGR * ang2 / OneRevolution );

    for (int i=0;i<2;i++) steppers[i]->run();

    
    // // calculate for the longest time required
    // double t_max = 0.0;
    // double t_accel = 0.5; // acceleration takes 0.5 second by default
    // for (int i=0;i<2;i++){
    //   double t_total = ((double) steppers[i]->distanceToGo() / (double) max_speeds[i]) + t_accel;
    //   t_max = max(t_max, t_total);
    // }

    // // set new speed for each stepper
    // for (int i=0;i<2;i++){
    //   steppers[i]->setMaxSpeed((double) steppers[i]->distanceToGo() / (t_max - t_accel));
    // }

    // // run both steppers
    // while (steppers[0]->run() && steppers[1]->run());
}

// reset both steppers to their home position via hall effect sensor
void DualStepper:: home(DualHallSensors& hallSensors){
    // yet to be implemented in version 2
}

void DualStepper:: set_max_speed(int speed1, int speed2){
  steppers[0]->setMaxSpeed(speed1);
  steppers[1]->setMaxSpeed(speed2);
}

void DualStepper:: set_acceleration(int accel1, int accel2){
  steppers[0]->setAcceleration(accel1);
  steppers[1]->setAcceleration(accel2);
}


