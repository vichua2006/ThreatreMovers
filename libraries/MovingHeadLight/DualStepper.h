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

// version 2
#ifdef USING_ACCELSTEPPER

class DualStepper {
  private: 
    AccelStepper &stepper1, &stepper2; 
    MultiStepper steppers;
    long target_positions[2]; 

  public:
    DualStepper(AccelStepper &s1, AccelStepper &s2);
    DualStepper() = default;

    void init_pin_mode();
    void turn_to(double ang1, double ang2);
    void home(DualHallSensors& hallSensors);
    void set_max_speed();
};

#endif

// version 1
#ifndef USING_ACCELSTEPPER
class StepperProperty{
  public:

    int STEPS;
    double GEAR_RATIO;
    double UPPER_BOUND, LOWER_BOUND;

    StepperProperty(int steps, double gr, double upper, double lower);
};

class StepperPins {
  public:

    int STEP_PIN, DIR_PIN, HALL_PIN, ENABLE_PIN;
    
    StepperPins(int step, int dir, int hall, int enable);
};

class StepperMotor {
  private:
    String MOTOR_NAME;
    int STEPS;
    int STEP_PIN, DIR_PIN, HALL_PIN;
    double GEAR_RATIO, CURR_POSITION; // current absolute position
    double UPPER_BOUND, LOWER_BOUND; // bounds on absolute position
    double STEPS_PER_DEGREE, DEGREES_PER_STEP;
    double SPEED; // in rpm

    static int ENABLE_PIN;

  public:
    StepperMotor(String name,
                 StepperPins PinObj,
                 StepperProperty PropObj); // constructor function
    
    StepperMotor() = default;

    void init_pin_mode();
    void turn(double deg, bool dir, int delay);
    // void turn_to(double position, int delay);
    void step(int delay);
    void set_position(double position);
    void set_direction(bool dir);
    void home(bool& homingSwitch);

    bool out_of_bounds(double position);

    int deg_to_step(double deg);
    int get_step_pin();

    double step_to_deg(int inc);
    // double min_angle_difference(double new_position);
    double get_angle_difference(double new_position);
    double get_position();
  
    String get_name();

    static void init_enable_pin();
};

class DualStepper {
  private: 
    StepperMotor &stepper1, &stepper2;  
    void swap_steppers();

  public:
    DualStepper(StepperMotor &s1, StepperMotor &s2);
    DualStepper() = default;

    void init_pin_mode();
    void turn(double deg1, double deg2, bool dir1, bool dir2); // bad implementation
    void turn_to(double pos1, double pos2);
    void home(DualHallSensors& hallSensors);
};
#endif


#endif