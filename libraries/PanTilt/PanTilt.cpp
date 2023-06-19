/* source file for pan tilt mechanism
 * Created by: Victor Huang
 * May 16, 2023
 */

#include "PanTilt.h"

// min/max need to be redefined as StandardCplusplus/ArduinoSTL undef'ed them
// this is copied from the arduino definition of min/max
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

const int INF = 1e9;

const int PanSteps = 200;
const int PanStepPin = 2;
const int PanDirPin = 5;
const int PanHallPin = 0;
const int PanAddress = 0;// eeprom address; undecided
const double PanGR = 144 / 17;
const double PanUpper = 359.0;
const double PanLower = -359.0;

const int TiltSteps = 200;
const int TiltStepPin = 3;
const int TiltDirPin = 6;
const int TiltHallPin = 0;
const int TiltAddress = 1;// eeprom address; undecided
const double TiltGR = 64 / 21;
const double TiltUpper = 89;
const double TiltLower = -89;

const int MotorEnablePin = 8;// enable pins for motor (cnc shield); must be at low
const int PotPin = A0; // pin for potentiometer

const int MinStepperDelay = 0;
const int FixedStepperDelay = 500;

String INPUT_STRING;
double INPUT_POS;

// create global stepper instances
StepperPins PanPins(PanStepPin, PanDirPin, PanHallPin, PanAddress);
StepperProperty PanProp(PanSteps, PanGR, PanUpper, PanLower);
StepperMotor pan("Pan stepper", PanPins, PanProp);

StepperPins TiltPins(TiltStepPin, TiltDirPin, TiltHallPin, TiltAddress);
StepperProperty TiltProp(TiltSteps, TiltGR, TiltUpper, TiltLower);
StepperMotor tilt("Tilt stepper", TiltPins, TiltProp);

/*---------------------------------------------------------------------class constructors---------------------------------------------------------------------------------*/

StepperPins:: StepperPins(int step, int dir, int hall, int eeprom_add){
  STEP_PIN = step;
  DIR_PIN = dir;
  HALL_PIN = hall;
  EEPROM_ADDRESS = eeprom_add;
}

StepperProperty:: StepperProperty(int steps, double gr, double upper, double lower){
  STEPS = steps;
  GEAR_RATIO = gr;
  UPPER_BOUND = upper;
  LOWER_BOUND = lower;
}

StepperMotor:: StepperMotor(String name,
                            StepperPins PinObj,
                            StepperProperty PropObj){
  MOTOR_NAME = name;

  STEPS = PropObj.STEPS;
  GEAR_RATIO = PropObj.GEAR_RATIO;
  UPPER_BOUND = PropObj.UPPER_BOUND;
  LOWER_BOUND = PropObj.LOWER_BOUND;

  STEP_PIN = PinObj.STEP_PIN;
  DIR_PIN = PinObj.DIR_PIN;
  HALL_PIN = PinObj.HALL_PIN;

  STEPS_PER_DEGREE = GEAR_RATIO * (double) STEPS / 360.0;
  DEGREES_PER_STEP = 360.0 / (GEAR_RATIO * (double) STEPS);

  CURR_POSITION = 0; // zero by default

  // CURR_POSITION = EEPROM.read(eeprom_address); // initalizes position by reading it from EEPROM: https://docs.arduino.cc/learn/built-in-libraries/eeprom
}

DualStepper:: DualStepper(const StepperMotor& s1, const StepperMotor& s2){
  stepper1 = s1;
  stepper2 = s2;
}

/*-----------------------------------------------------------------------class methods-------------------------------------------------------------------------------*/


void StepperMotor:: init_pin_mode(){
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(HALL_PIN, INPUT);
}

// hall effect sensor initalization; usage still uncertain
void StepperMotor:: init_pos(){

}


// turns the motors some degrees in specified direction with a delay between steps, and directly modifies the motor's absolute position
// deg_difference can be positive or negative; only magnitude matters
void StepperMotor:: turn(double deg_difference, bool dir, int delay = FixedStepperDelay){
  
  double unsigned_difference = abs(deg_difference);

  if (unsigned_difference < DEGREES_PER_STEP) return ; // exit function if motor doesn't need to turn

  // compute the number of steps
  int increments = deg_to_step(unsigned_difference);

  int delay_per_step = max(delay, MinStepperDelay); // calculate delay
  // 500 microseconds is pretty much the limit; anything faster the motor doesn't pick up

  if (dir == 1) digitalWrite(DIR_PIN, HIGH); // set rotation direction
  else digitalWrite(DIR_PIN, LOW);

  for (int i=0;i<increments;i++){
    this->step(delay);
  }

  // compute actual angular displacement
  double angular_displacement = step_to_deg(increments);
  // the stepper may not actually be able to turn exactly deg_difference, but only close to it
  // so the position must be determined by the number of steps it actually takes to avoid error accumulation

  // if direction is ccw
  if (dir == 0) angular_displacement = -angular_displacement;

  // modify absolute position
  CURR_POSITION += angular_displacement;
  
}

// turns the motor to some absolute angular position
void StepperMotor:: turn_to(double new_position, int delay = FixedStepperDelay){
  if (out_of_bounds(new_position)){
    Serial.println(MOTOR_NAME + " OUTSIDE RANGE OF MOTION");
    return ;
  }

  double deg_difference = this->min_angle_difference(new_position);
  bool direction = (deg_difference > 0 ? 1 : 0); // 1 is cw, 0 ccw

  this->turn(deg_difference, direction, delay);// turn motor

  
}


// make stepper turn by one step
void StepperMotor:: step(int delay){
  digitalWrite(STEP_PIN, HIGH); 
  delayMicroseconds(delay);
  digitalWrite(STEP_PIN, LOW); 
  delayMicroseconds(delay); 
}

bool StepperMotor:: out_of_bounds(double position){
  return position < LOWER_BOUND || position > UPPER_BOUND;
}

// converts the degree difference to number of steps
int StepperMotor:: deg_to_step(double deg){
  return deg * STEPS_PER_DEGREE;
}

// converts increment difference to number of degrees
double StepperMotor:: step_to_deg(int inc){
  return (double) inc * DEGREES_PER_STEP;
}

// given a relative position within bounds, returns the angle and direction the stepper needs to turn, taking into consideration its absolute position.
// positive is clockwise, negative is ccw
double StepperMotor:: min_angle_difference(double position){

  double curr_relative_pos = CURR_POSITION; // calculate the current relative position
  if (curr_relative_pos < 0) curr_relative_pos += 360.0;

  double new_position = position - curr_relative_pos; // effectively set current relative position to 0
  if (new_position < 0) new_position += 360.0; 

  double ang_difference1 = new_position; // clockwise distance
  double ang_difference2 = new_position - 360.0; // counter-clockwise

  // assume that angdiff1 is always more optimal
  if (abs(ang_difference1) > abs(ang_difference2)){
    double temp = ang_difference1;
    ang_difference1 = ang_difference2;
    ang_difference2 = temp;
  }

  if (out_of_bounds(CURR_POSITION + ang_difference1)) return ang_difference2;
  else return ang_difference1;
}

double StepperMotor:: get_position(){
  return CURR_POSITION;
}

/*--------------------------------------------------------------------general functions----------------------------------------------------------------------------------*/


void initPanTilt(){
  
  // enable pins for motor (cnc shield); must be at low
  pinMode(MotorEnablePin, OUTPUT);
  digitalWrite(MotorEnablePin, LOW);

  pinMode(PotPin, INPUT);

  pan.init_pin_mode();
  tilt.init_pin_mode();

}

double readSerialInput(){
  

}

void mainLoop(){

  if (Serial.available()){
    INPUT_STRING = Serial.readStringUntil('\n');
    INPUT_POS = INPUT_STRING.toDouble();

    Serial.println(tilt.min_angle_difference(INPUT_POS));

    tilt.turn_to(INPUT_POS);
    
    Serial.println(tilt.MOTOR_NAME + " at position " + tilt.get_position()); // debugging
  }
}

