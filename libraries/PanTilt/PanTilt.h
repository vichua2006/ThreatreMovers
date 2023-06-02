/* header file for pan tilt mechanism
 * variables and functions are only delcared; they are defined in sources file
 * Created by: Victor Huang
 * May 16, 2023
 */
#ifndef PanTilt_h
#define PanTilt_h

#include "Arduino.h"

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/

class Motor {
  public:

    String MOTOR_NAME;
    int STEPS;
    int STEP_PIN, DIR_PIN, HALL_PIN, EEPROM_ADDRESS;
    double GEAR_RATIO, CURR_POSITION; // position in degrees

    Motor(String name,
          int steps, int step_pin, int dir_pin, 
          int hall_pin, int eeprom_address, 
          double gr); // constructor function

    void init_pin_mode();
    void init_pos();
    void turn(double deg, bool dir, double time);
    void turn_to(double angular_position, double time);
    int deg_to_step(double deg);
};

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/

void initPanTilt();
void mainLoop();

double readSerialInput();

#endif