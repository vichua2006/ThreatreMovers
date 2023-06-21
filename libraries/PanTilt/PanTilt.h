/* header file for pan tilt mechanism
 * Created by: Victor Huang and Rohan Katreddy
 * May 16, 2023
 */

/* Change log
 * 2023-06-21:
 *     Added Hall sensor funcitionality and homing function.
 */

#ifndef PanTilt_h
#define PanTilt_h

#include <Arduino.h>

// header needs to be kept in so that arduino-cli does not automatically delete path from c_cpp_properties.json
// #include <ArduinoSTL.h>

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
    double GEAR_RATIO, CURR_POSITION; // current absolute position
    double UPPER_BOUND, LOWER_BOUND; // bounds on absolute position
    double STEPS_PER_DEGREE, DEGREES_PER_STEP;

    StepperMotor(String name,
                 StepperPins PinObj,
                 StepperProperty PropObj); // constructor function
    
    StepperMotor() = default;

    void init_pin_mode();
    void init_pos();

    void turn(double deg, bool dir, int delay);
    void turn_to(double position, int delay);
    void step(int delay);
    void set_position(double position);
    void set_direction(bool dir);
    void home(bool& homingSwitch);

    bool out_of_bounds(double position);

    int deg_to_step(double deg);

    double step_to_deg(int inc);
    double min_angle_difference(double new_position);
    double get_position();
  
    String get_name();
};

class DualStepper {
  public:

    StepperMotor &stepper1, &stepper2;

    int TURN_SEGMENTS; // specifies how many parts/segments a turn is broken into
    // larger the number, smoother the movement (to be tested)

    DualStepper(StepperMotor &s1, StepperMotor &s2, int seg);

    void turn(double deg1, double deg2, bool dir1, bool dir2, int delay); // bad implementation
    void turn_to(double pos1, double pos2, int delay);
    void home(DualHallSensors hallSensors);
};

class DualHallSensors{
  // Private class to be used by parent class.
  private:
    class HallSensor{
      public:
        HallSensor(int pin);
        HallSensor() = default; // default constructor for
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
    // Refrain from functions as interrupt variables are prefered.
    int readPanHall();
    int readTiltHall();
    // Use below interrupt variables to check state of hall effects.
    
    static bool isPanHallClosed;
    static bool isTiltHallClosed;
};


/*------------------------------------------------------------------------------------------------------------------------------------------------------*/

void initPanTilt();
void mainLoop();


double readSerialInput();

#endif