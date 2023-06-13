/* header file for pan tilt mechanism
 * Created by: Victor Huang
 * May 16, 2023
 */
#ifndef PanTilt_h
#define PanTilt_h

#include "Arduino.h"

// header needs to be kept in so that arduino-cli does not automatically delete path from c_cpp_properties.json
#include <ArduinoSTL.h>
#include <cstdarg>
#include <vector>

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/

class StepperProperty{
  public:

    int STEPS;
    double GEAR_RATIO;

    StepperProperty(int steps, double gr);
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
    double GEAR_RATIO, CURR_POSITION; // position in degrees

    StepperMotor(String name,
          StepperPins PinObj,
          StepperProperty PropObj); // constructor function

    void init_pin_mode();
    void init_pos();
    void turn(double deg, bool dir, double time);
    void turn_to(double angular_position, double time);
    int deg_to_step(double deg);
};

class MultiStepper {
  public:

    std:: vector<StepperMotor> steppers;

    // const char* is a 
    MultiStepper(const char* fmt, ...){

    }
};

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/

void initPanTilt();
void mainLoop();

double readSerialInput();

#endif