/* source file for pan tilt mechanism
 * Created by: Victor Huang and Rohan Katreddy
 * May 16, 2023
 */

#include "DualStepper.h"
#include "GeneralFunc.h"
#include "config.h"
#include <Arduino.h>

// version 2
#ifdef USING_ACCELSTEPPER

DualStepper:: DualStepper(AccelStepper &s1, AccelStepper &s2) : stepper1(s1), stepper2(s2) {
  this->set_max_speed();
  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
}

// enable pins for motor (cnc shield); must be at low
void DualStepper:: init_pin_mode(){
    // AccelStepper will self initalize
    
    pinMode(StepperEnablePin, OUTPUT);
    digitalWrite(StepperEnablePin, LOW);
}

// turns both steppers to absolute position, finishing near the same time
void DualStepper:: turn_to(double ang1, double ang2){
    
    // convert degrees to number of steps, taking into account motor gear ratio and (implicitly) microsteps
    target_positions[0] = (double) PanSteps * PanGR * ang1 / OneRevolution;
    target_positions[1] = (double) TiltSteps * TiltGR * ang2 / OneRevolution ;

    steppers.moveTo(target_positions);
    steppers.runSpeedToPosition();
}

// reset both steppers to their home position via hall effect sensor
void DualStepper:: home(DualHallSensors& hallSensors){
    // yet to be implemented in version 2
}

void DualStepper:: set_max_speed(){
    stepper1.setMaxSpeed(PanMaxSpeed);
    stepper2.setMaxSpeed(TiltMaxSpeed);
}


#endif

// version 1
#ifndef USING_ACCELSTEPPER

StepperPins:: StepperPins(int step, int dir, int hall, int enable){
  STEP_PIN = step;
  DIR_PIN = dir;
  HALL_PIN = hall;

  ENABLE_PIN = enable;
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
  ENABLE_PIN = PinObj.ENABLE_PIN;

  STEPS_PER_DEGREE = GEAR_RATIO * (double) STEPS / OneRevolution;
  DEGREES_PER_STEP = OneRevolution / (GEAR_RATIO * (double) STEPS);
  SPEED = 10; // default value

  CURR_POSITION = 0; // zero by default
}

DualStepper:: DualStepper(StepperMotor &s1, StepperMotor &s2) : stepper1(s1), stepper2(s2) {
  // more on reference member initalization: https://stackoverflow.com/questions/30069384/provides-no-initializer-for-reference-member
  // on member initalizer list: https://stackoverflow.com/questions/1711990/what-is-this-weird-colon-member-syntax-in-the-constructor
}

// initialize arduino pins for stepper
void StepperMotor:: init_pin_mode(){
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(HALL_PIN, INPUT);
}

// turns the motors some degrees in specified direction with a delay between steps, and directly modifies the motor's absolute position
// deg_difference can be positive or negative; only magnitude matters
void StepperMotor:: turn(double deg_difference, bool dir, int delay = FixedStepperDelay){
  
  double unsigned_difference = abs(deg_difference);

  if (unsigned_difference < DEGREES_PER_STEP) return ; // exit function if motor doesn't need to turn

  // compute the number of steps
  int increments = deg_to_step(unsigned_difference);

  int delay_per_step = FixedStepperDelay; // calculate delay
  // 500 microseconds is pretty much the limit; anything faster the motor doesn't pick up

  this->set_direction(dir);

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
  this->set_position(CURR_POSITION + angular_displacement);

  // Serial.println(MOTOR_NAME + " at position " + CURR_POSITION); // debugging
}

// DEPRECATED. DO NOT USE
// turns the motor to some absolute angular position
// void StepperMotor:: turn_to(double new_position, int delay = FixedStepperDelay){
//   if (new_position < 0){
//     // Serial.println("POSITION CANNOT BE NEGATIVE");
//     return ;
//   }
//   if (out_of_bounds(new_position)){
//     // Serial.println(this->get_name() + " POSITION OUT OF RANGE");
//     return ;
//   }

//   double deg_difference = this->min_angle_difference(new_position);
//   bool direction = (deg_difference > 0 ? 1 : 0); // 1 is cw, 0 ccw

//   this->turn(deg_difference, direction, delay);// turn motor
// }


// make stepper turn by one step
void StepperMotor:: step(int delay = FixedStepperDelay){
  digitalWrite(STEP_PIN, HIGH); 
  delayMicroseconds(delay);
  digitalWrite(STEP_PIN, LOW); 
  delayMicroseconds(delay); 
}

// set stepper position 
void StepperMotor:: set_position(double position){
  CURR_POSITION = position;
}

// set stepper roation direction
void StepperMotor:: set_direction(bool dir){
  if (dir == 1) digitalWrite(DIR_PIN, HIGH); // clockwise
  else digitalWrite(DIR_PIN, LOW); // ccw
}

// bad implementation; fix later
void StepperMotor:: home(bool& homingSwitch){
  set_direction(1);
  while (!homingSwitch){
    step();
  }
}

// checks if the given position is out of bounds with uncertainty
bool StepperMotor:: out_of_bounds(double position){
  return position < LOWER_BOUND - BoundaryUC || position > UPPER_BOUND + BoundaryUC;
}

// converts the degree difference to number of steps
int StepperMotor:: deg_to_step(double deg){
  return deg * STEPS_PER_DEGREE;
}

int StepperMotor:: get_step_pin(){
  return STEP_PIN;
}

// converts increment difference to number of degrees
double StepperMotor:: step_to_deg(int inc){
  return (double) inc * DEGREES_PER_STEP;
}

// NOT USE. pan no longer supports 720 degree range; only 360
// given a relative position within bounds, returns the angle and direction the stepper needs to turn, taking into consideration its absolute position.
// positive is clockwise, negative is ccw
// double StepperMotor:: min_angle_difference(double position){

//   double curr_relative_pos = CURR_POSITION; // calculate the current relative position
//   if (curr_relative_pos < 0) curr_relative_pos += OneRevolution;
//   // if (position < 0) position += OneRevolution;

//   double new_position = position - curr_relative_pos; // effectively set current relative position to 0
//   if (new_position < 0) new_position += OneRevolution; 

//   double ang_difference1 = new_position; // clockwise distance
//   double ang_difference2 = new_position - OneRevolution; // counter-clockwise

//   // assume that angdiff1 is always more optimal
//   if (abs(ang_difference1) > abs(ang_difference2)){
//     swap(ang_difference1, ang_difference2);
//   }

//   if (out_of_bounds(CURR_POSITION + ang_difference1)) return ang_difference2;
//   else return ang_difference1;
// }

// returns the angle and direction the stepper needs to turn
// positive is clockwise, negative is ccw
double StepperMotor:: get_angle_difference(double new_position){
  return new_position - CURR_POSITION;
}

double StepperMotor:: get_position(){
  return CURR_POSITION;
}

String StepperMotor:: get_name(){
  return MOTOR_NAME;
}

void StepperMotor:: init_enable_pin(){
  // enable pins for motor (cnc shield); must be at low
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW);
}

// static variable initialization
int StepperMotor:: ENABLE_PIN;

// swap stepper references. for internal usage.
void DualStepper:: swap_steppers(){
  swap(stepper1, stepper2);
}

void DualStepper:: init_pin_mode(){
  stepper1.init_pin_mode();
  stepper2.init_pin_mode();
  stepper1.init_enable_pin();
}

// turns both steppers simultaneously, finishing at the same time
void DualStepper:: turn(double deg1, double deg2, bool dir1, bool dir2){

  double unsigned_difference1 = abs(deg1);
  double unsigned_difference2 = abs(deg2);

  int inc1 = stepper1.deg_to_step(unsigned_difference1);
  int inc2 = stepper2.deg_to_step(unsigned_difference2);

  int delay_per_step = FixedStepperDelay;

  stepper1.set_direction(dir1);
  stepper2.set_direction(dir2);

  bool swapped = false;
  if (inc1 < inc2){
    swapped = true;
    swap(inc1, inc2);
    this->swap_steppers();
  }

  // ratio needs to be precise, or else stepper with smaller increment will turn too much
  double ratio = (double) inc1 / (double) inc2;

  // essentially, stepper2 steps once every {ratio} times stepper1 steps
  for (int i=0,j=0;i<inc1;i++){
    digitalWrite(stepper1.get_step_pin(), HIGH);
    // for whatever reason, if inc2 == 0, ratio is then -1, causing stepper2 to step every step.
    if (i >= (double) j * ratio && inc2 != 0){
      digitalWrite(stepper2.get_step_pin(), HIGH);
      j ++ ;
    }
    delayMicroseconds(delay_per_step);
    digitalWrite(stepper1.get_step_pin(), LOW);
    digitalWrite(stepper2.get_step_pin(), LOW); 
    delayMicroseconds(delay_per_step);
  }

  if (swapped){
    // swap back
    swap(inc1, inc2);
    this->swap_steppers();
  }

  double angular_displacement1 = stepper1.step_to_deg(inc1);
  double angular_displacement2 = stepper2.step_to_deg(inc2);

  // if direction is ccw
  if (dir1 == 0) angular_displacement1 = -angular_displacement1;
  if (dir2 == 0) angular_displacement2 = -angular_displacement2;

  stepper1.set_position(stepper1.get_position() + angular_displacement1);
  stepper2.set_position(stepper2.get_position() + angular_displacement2);

  // Serial.println(stepper1.get_name() + " at position " + stepper1.get_position()); // debugging
  // Serial.println(stepper2.get_name() + " at position " + stepper2.get_position());
}

// turn both steppers simultaneously
void DualStepper:: turn_to(double pos1, double pos2){
  if (pos1 < 0 || pos2 < 0){
    // Serial.println("POSITION CANNOT BE NEGATIVE");
    return ;
  }
  if (stepper1.out_of_bounds(pos1) || stepper2.out_of_bounds(pos2)){
    String msg = "";
    if (stepper1.out_of_bounds(pos1)) msg += stepper1.get_name() + " ";
    if (stepper2.out_of_bounds(pos2)) msg += stepper2.get_name() + " ";

    // Serial.println(msg + "POSITION OUT OF RANGE");
    return ;
  }

  // compute degree difference for first stepper
  double deg_diff1 = stepper1.get_angle_difference(pos1);
  bool dir1 = (deg_diff1 > 0 ? 1 : 0); 

  // for second stepper
  double deg_diff2 = stepper2.get_angle_difference(pos2);
  bool dir2 = (deg_diff2 > 0 ? 1 : 0);

  this->turn(deg_diff1, deg_diff2, dir1, dir2);
}

void DualStepper:: home(DualHallSensors& hallSensors){
  stepper1.home(hallSensors.isPanHallClosed);
  stepper2.home(hallSensors.isTiltHallClosed);
}

#endif
