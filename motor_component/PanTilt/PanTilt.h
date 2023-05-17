/* header file for pan tilt mechanism
 * variables and functions are only delcared; they are defined in sources file
 * Created by: Victor Huang
 * May 16, 2023
 */
#ifndef PanTilt_h
#define PanTilt_h

#include "Arduino.h"

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/

struct Motor {
  public:
    int STEP_PIN, DIR_PIN, HALL_PIN, EEPROM_ADDRESS;
    double GEAR_RATIO, POSITION; // position in degrees
    Motor(int step, int dir, int hall, int eeprom_address, double gr); // constructor function to be defined
    void init_pin_mode();
    void init_pos();
    void turn(double deg, bool dir, double time);
    void turn_to();
};

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/

void initPanTilt();
void degToStep();

#endif