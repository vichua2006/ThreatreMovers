/* source file for hall effect sensor positioning/homing mechanism
 * Created by: Victor Huang and Rohan Katreddy
 * May 16, 2023
 */

#include "PanTilt.h" 

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
const int FixedStepperDelay = 800;

const int PanTiltSegment = 1000;

String INPUT_STRING;
double INPUT_POS;

// create global stepper instances
StepperPins PanPins(PanStepPin, PanDirPin, PanHallPin, PanAddress);
StepperProperty PanProp(PanSteps, PanGR, PanUpper, PanLower);
StepperMotor pan("Pan stepper", PanPins, PanProp);

StepperPins TiltPins(TiltStepPin, TiltDirPin, TiltHallPin, TiltAddress);
StepperProperty TiltProp(TiltSteps, TiltGR, TiltUpper, TiltLower);
StepperMotor tilt("Tilt stepper", TiltPins, TiltProp);

DualStepper pan_tilt(pan, tilt);

DualHallSensors pan_tilt_sensors(PanHallPin, TiltHallPin);

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

DualStepper:: DualStepper(StepperMotor &s1, StepperMotor &s2) : stepper1(s1), stepper2(s2) {
  // more on reference member initalization: https://stackoverflow.com/questions/30069384/provides-no-initializer-for-reference-member
  // on member initalizer list: https://stackoverflow.com/questions/1711990/what-is-this-weird-colon-member-syntax-in-the-constructor
}

DualHallSensors:: DualHallSensors(int panPin, int tiltPin){

  // initialize HallSensor instances
  HallSensor panHall(panPin);
  HallSensor tiltHall(tiltPin);

  attachInterrupt(digitalPinToInterrupt(panPin), pan_hall_fallingISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(panPin), pan_hall_risingISR, RISING);
  attachInterrupt(digitalPinToInterrupt(tiltPin), tilt_hall_fallingISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(tiltPin), tilt_hall_risingISR, RISING);
};

DualHallSensors:: HallSensor:: HallSensor(int pin){
  PIN = pin;
}

/*-----------------------------------------------------------------------class methods-------------------------------------------------------------------------------*/


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

  int delay_per_step = max(delay, MinStepperDelay); // calculate delay
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

  Serial.println(MOTOR_NAME + " at position " + CURR_POSITION); // debugging
}

// turns the motor to some absolute angular position
void StepperMotor:: turn_to(double new_position, int delay = FixedStepperDelay){
  if (out_of_bounds(new_position)){
    Serial.println(get_name() + " POSITION OUT OF RANGE");
    return ;
  }

  double deg_difference = this->min_angle_difference(new_position);
  bool direction = (deg_difference > 0 ? 1 : 0); // 1 is cw, 0 ccw

  this->turn(deg_difference, direction, delay);// turn motor
}


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

bool StepperMotor:: out_of_bounds(double position){
  return position < LOWER_BOUND || position > UPPER_BOUND;
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

String StepperMotor:: get_name(){
  return MOTOR_NAME;
}


void DualStepper:: turn(double deg1, double deg2, bool dir1, bool dir2, int delay){
  // ugly code; will optimise later
  // basically StepperMotor:: turn(), but for two steppers instead

  StepperMotor &ts1 = stepper1, &ts2 = stepper2;

  double unsigned_difference1 = abs(deg1);
  double unsigned_difference2 = abs(deg2);

  int inc1 = ts1.deg_to_step(unsigned_difference1);
  int inc2 = ts2.deg_to_step(unsigned_difference2);

  int delay_per_step = max(delay, MinStepperDelay); 

  ts1.set_direction(dir1);
  ts2.set_direction(dir2);

  if (inc1 < inc2){
    int tinc = inc1;
    inc1 = inc2;
    inc2 = tinc;

    StepperMotor &ts = ts1;
    ts1 = ts2;
    ts2 = ts;
  }

  for (int i=0,j=0;i<inc1;i++){
    digitalWrite(ts1.get_step_pin(), HIGH);
    if (i >= (int) j * inc1 / inc2){
      digitalWrite(ts2.get_step_pin(), HIGH);
      j ++ ;
    }
    delayMicroseconds(delay_per_step);
    digitalWrite(ts1.get_step_pin(), LOW);
    digitalWrite(ts2.get_step_pin(), LOW); 
    delayMicroseconds(delay_per_step);
  }

  double angular_displacement1 = ts1.step_to_deg(inc1);
  double angular_displacement2 = ts2.step_to_deg(inc2);

  // if direction is ccw
  if (dir1 == 0) angular_displacement1 = -angular_displacement1;
  if (dir2 == 0) angular_displacement2 = -angular_displacement2;

  ts1.set_position(ts1.get_position() + angular_displacement1);
  ts2.set_position(ts2.get_position() + angular_displacement2);

  Serial.println(stepper1.get_name() + " at position " + stepper1.get_position()); // debugging
  Serial.println(stepper2.get_name() + " at position " + stepper2.get_position());
}

// turn both steppers simultaneously
void DualStepper:: turn_to(double pos1, double pos2, int delay = FixedStepperDelay){
  // checks for boundary
  if (stepper1.out_of_bounds(pos1) || stepper2.out_of_bounds(pos2)){
    String msg = "";
    if (stepper1.out_of_bounds(pos1)) msg += stepper1.get_name() + " ";
    if (stepper2.out_of_bounds(pos2)) msg += stepper2.get_name() + " ";

    Serial.println(msg + "POSITION OUT OF RANGE");
    return;
  }

  // compute degree difference for first stepper
  double deg_diff1 = stepper1.min_angle_difference(pos1);
  bool dir1 = (deg_diff1 > 0 ? 1 : 0); 

  // for second stepper
  double deg_diff2 = stepper2.min_angle_difference(pos2);
  bool dir2 = (deg_diff2 > 0 ? 1 : 0);

  this->turn(deg_diff1, deg_diff2, dir1, dir2, delay);
}

void DualStepper:: home(DualHallSensors& hallSensors){
  stepper1.home(hallSensors.isPanHallClosed);
  stepper2.home(hallSensors.isTiltHallClosed);
}

int DualHallSensors:: HallSensor:: read(){
  return digitalRead(this->PIN);
}

int DualHallSensors:: readPanHall(){
  return panHall.read();
}

int DualHallSensors:: readTiltHall(){
  return tiltHall.read();
}

void DualHallSensors:: pan_hall_fallingISR(){
  isPanHallClosed = true;
}

void DualHallSensors:: pan_hall_risingISR(){
  isPanHallClosed = false;
}

void DualHallSensors:: tilt_hall_fallingISR(){
  isTiltHallClosed = true;
}

void DualHallSensors:: tilt_hall_risingISR(){
  isTiltHallClosed = false;
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

    pan_tilt.turn_to(INPUT_POS, INPUT_POS / 4);
  }
}

