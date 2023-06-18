/* header file for pan tilt mechanism
 * Created by: Victor Huang
 * May 16, 2023
 */
#ifndef PanTilt_h
#define PanTilt_h

#include <Arduino.h>

// // header needs to be kept in so that arduino-cli does not automatically delete path from c_cpp_properties.json
// #include <ArduinoSTL.h>
// #include <cstdarg>
// #include <vector>

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/

class StepperProperty{
  public:

    int STEPS;
    double GEAR_RATIO;
    double UPPER_BOUND, LOWER_BOUND;

    StepperProperty(int steps, double gr, double upper, double lower);
};

class StepperPins {
  public:

    int STEP_PIN, DIR_PIN, HALL_PIN, EEPROM_ADDRESS;
    
    StepperPins(int step, int dir, int hall, int eeprom_add);
};

class StepperMotor {
  public:

    String MOTOR_NAME;
    int STEPS;
    int STEP_PIN, DIR_PIN, HALL_PIN, EEPROM_ADDRESS;
    double GEAR_RATIO, CURR_POSITION;
    double UPPER_BOUND, LOWER_BOUND; // bounds on absolute position
    double STEPS_PER_DEGREE, DEGREES_PER_STEP;

    StepperMotor(String name,
                 StepperPins PinObj,
                 StepperProperty PropObj); // constructor function
    
    StepperMotor() = default;

    void init_pin_mode();
    void init_pos();
    void turn(double deg, bool dir, int delay);
    void turn_to(double angular_position, int delay);
    void step(int delay);

    int deg_to_step(double deg);
    double step_to_deg(int inc);
};

class DualStepper {
  public:

    StepperMotor stepper1, stepper2;

    DualStepper(const StepperMotor& s1, const StepperMotor& s2);

    void turn_to(double angle1, double angle2);
};

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/

void initPanTilt();
void mainLoop();


double readSerialInput();

#endif