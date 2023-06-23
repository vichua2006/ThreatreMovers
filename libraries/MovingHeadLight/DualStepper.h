/* header file for pan tilt mechanism
 * Created by: Victor Huang and Rohan Katreddy
 * May 16, 2023
 *
 * 
 * 2023-06-21: Added homing function.
 *     
 */

#ifndef DualStepper_h
#define DualStepper_h

#include <Arduino.h>
#include "HallSensor.h"

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

    static int ENABLE_PIN;

  public:
    StepperMotor(String name,
                 StepperPins PinObj,
                 StepperProperty PropObj); // constructor function
    
    StepperMotor() = default;

    void init_pin_mode();
    void turn(double deg, bool dir, int delay);
    void turn_to(double position, int delay);
    void step(int delay);
    void set_position(double position);
    void set_direction(bool dir);
    void home(bool& homingSwitch);

    bool out_of_bounds(double position);

    int deg_to_step(double deg);
    int get_step_pin();

    double step_to_deg(int inc);
    double min_angle_difference(double new_position);
    double get_position();
  
    String get_name();

    static void init_enable_pin();
};

class DualStepper {
  private: 
    StepperMotor &stepper1, &stepper2;  

  public:
    DualStepper(StepperMotor &s1, StepperMotor &s2);
    DualStepper() = default;

    void init_pin_mode();
    void turn(double deg1, double deg2, bool dir1, bool dir2, int delay); // bad implementation
    void turn_to(double pos1, double pos2, int delay);
    void home(DualHallSensors& hallSensors);
};


#endif