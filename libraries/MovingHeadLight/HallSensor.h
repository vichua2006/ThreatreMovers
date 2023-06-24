/* header file for hall effect sensor positioning/homing mechanism
 * Created by: Victor Huang and Rohan Katreddy
 * June 21, 2023
 *
 * 2023-06-22: Defined static member in source file to avoid undefined reference
 */

#ifndef HallSensor_h
#define HallSensor_h

class DualHallSensors{
  // Private class to be used by parent class.
  private:
    class HallSensor{
      public:
        HallSensor(int pin);
        HallSensor() = default; 
        void init_pin_mode();
        int read();
      private:
        int PIN;
    };
    
    HallSensor panHall;
    HallSensor tiltHall;

    static void pan_hall_fallingISR();
    static void pan_hall_risingISR();
    static void tilt_hall_fallingISR();
    static void tilt_hall_risingISR();

  // Methods to be used by user.
  public:
    DualHallSensors(int panPin, int tiltPin);
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