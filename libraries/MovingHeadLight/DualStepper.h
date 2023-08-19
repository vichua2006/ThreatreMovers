/* header file for pan tilt mechanism
 * Created by: Victor Huang and Rohan Katreddy
 * May 16, 2023
 *
 * 
 * 2023-06-21: Added homing function.
 * 2023-06-22: Defined static member in source file to avoid undefined reference
 * 2023-06-23: Added swap_stepper function and debugged Dualstepper methods
 * 2023-08-14: Reduced/increased range of pan/tilt & set fixed delay
 * 2023-08-15: Added an alternate version implemented with AccelStepper
 */

#ifndef DualStepper_h
#define DualStepper_h

#include "HallSensor.h"
#include "config.h"
#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>


class DualStepper {
  private: 
    AccelStepper* steppers[2];
    int max_speeds[2]; 

  public:
    DualStepper(AccelStepper &s1, AccelStepper &s2);
    DualStepper() = default;

    void init_pin_mode();
    void step_towards(double ang1, double ang2);
    void home(DualHallSensors& hallSensors);
    void set_max_speed(int speed1, int speed2);
    void set_acceleration(int accel1, int accel2);
};

#endif