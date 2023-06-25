/* header file for hall effect sensor positioning/homing mechanism
 * Created by: Victor Huang and Rohan Katreddy
 * June 21, 2023
 *
 * 2023-06-22: Defined static member in source file to avoid undefined reference
 * 2023-06-25: changed HallSenor class to public for debugging, changed IRS to change mode
 * 
 */

#ifndef HallSensor_h
#define HallSensor_h

class HallSensor{
  public:
    int PIN;

    HallSensor(int pin);
    HallSensor() = default; 
    
    void init_pin_mode();
    
    int get_pin();
    int read();
    
};
    

class DualHallSensors{

  public:

    HallSensor& panHall;
    HallSensor& tiltHall;

    static void pan_hall_changingISR();
    static void tilt_hall_changingISR();

    DualHallSensors(HallSensor& PanH, HallSensor& TiltH);
    DualHallSensors() = default;
    
    void init_pin_mode();
    // Refrain from functions as interrupt variables are prefered.
    int read_pan_hall();
    int read_tilt_hall();

    // Use below interrupt variables to check state of hall effects.
    static bool isPanHallClosed;
    static bool isTiltHallClosed;
};

#endif