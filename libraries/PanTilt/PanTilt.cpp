/* source file for pan tilt mechanism
 * Created by: Victor Huang
 * May 16, 2023
 */

#include "PanTilt.h"

// min/max need to be redefined as StandardCplusplus/ArduinoSTL undef'ed them
// this is copied from the arduino definition of min/max
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))


const int PanSteps = 200;
const int PanStepPin = 2;
const int PanDirPin = 5;
const int PanHallPin = 0;
const int PanAddress = 0;// eeprom address; undecided
const double PanGR = 144 / 17;
const double PanUpper = 360;
const double PanLower = -360;

const int TiltSteps = 200;
const int TiltStepPin = 3;
const int TiltDirPin = 6;
const int TiltHallPin = 0;
const int TiltAddress = 1;// eeprom address; undecided
const double TiltGR = 64 / 21;
const double TiltUpper = 90;
const double TiltLower = -90;

const int MotorEnablePin = 8;// enable pins for motor (cnc shield); must be at low
const int PotPin = A0;

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

// turns the motors some degrees in specified direction with a delay between steps, and directly modifies the motor's absolute position
void StepperMotor:: turn(double deg_difference, bool dir, int delay = FixedStepperDelay){
  if (deg_difference < DEGREES_PER_STEP) return ; // exit function if motor doesn't need to turn

  // compute the number of steps
  int increments = deg_to_step(deg_difference);

  // compute actual angular displacement
  double angular_displacement = step_to_deg(increments);
  // the stepper may not actually be able to turn exactly deg_difference, but only close to it
  // so the position must be determined by the number of steps it actually takes to avoid error accumulation

  // modify absolute position
  
  int delay_per_step = max(delay, MinStepperDelay); // calculate delay
  // 500 microseconds is pretty much the limit; anything faster the motor doesn't pick up


  if (dir == 1) digitalWrite(DIR_PIN, HIGH); // set rotation direction
  else digitalWrite(DIR_PIN, LOW);

  for (int i=0;i<increments;i++){
    this->step(delay);
  }
}

// turns the motor to some absolute angular position
void StepperMotor:: turn_to(double angular_position, int delay = FixedStepperDelay){
  if (angular_position < 0 || angular_position > 180){
    Serial.println("OUTSIDE RANGE OF MOTION");
    return ;
  }

  bool direction = (angular_position >= CURR_POSITION ? 1 : 0); // 1 is cw, 0 ccw
  double deg_difference = abs(angular_position - CURR_POSITION);

  this->turn(deg_difference, direction, delay);// turn motor

  Serial.println("Motor " + MOTOR_NAME + " at position " + CURR_POSITION); // debugging
}

// hall effect sensor initalization; usage still uncertain
void StepperMotor:: init_pos(){

}

// make stepper turn by one step
void StepperMotor:: step(int delay){
  digitalWrite(STEP_PIN, HIGH); 
  delayMicroseconds(delay);
  digitalWrite(STEP_PIN, LOW); 
  delayMicroseconds(delay); 
}

// converts the degree difference to number of steps taken by the driver motor
int StepperMotor:: deg_to_step(double deg){
  return deg * STEPS_PER_DEGREE;
}

double StepperMotor:: step_to_deg(int inc){
  return (double) inc * DEGREES_PER_STEP;
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
  if (Serial.available()){
    INPUT_STRING = Serial.readStringUntil('\n');
    INPUT_POS = INPUT_STRING.toDouble();
    
    pan.turn_to(INPUT_POS, 0.7);
    tilt.turn_to(INPUT_POS, 0.2);
  }

}

void mainLoop(){

  int pot_val = analogRead(A0);

  double angle = map(pot_val, 0, 1023, 0, 180);
  pan.turn_to(angle);
}

