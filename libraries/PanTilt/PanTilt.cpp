#include "Arduino.h"
#include "PanTilt.h"
#include <EEPROM.h>

const int PanSteps = 200;
const int PanStepPin = 2;
const int PanDirPin = 5;
const int PanHallPin = 0;
const int PanAddress = 0;// eeprom address; undecided
const double PanGR = 144 / 17;

const int TiltSteps = 200;
const int TiltStepPin = 3;
const int TiltDirPin = 6;
const int TiltHallPin = 0;
const int TiltAddress = 1;// eeprom address; undecided
const double TiltGR = 64 / 21;

const int MotorEnablePin = 8;// enable pins for motor (cnc shield); must be at low
const int PotPin = A0;

const int MinMotorDelay = 1000;

String INPUT_STRING;
double INPUT_POS;

Motor pan("Pan motor", 
            PanSteps, PanStepPin,
            PanDirPin, PanHallPin, PanAddress,
            PanGR);

Motor tilt("Tilt motor", 
            TiltSteps, TiltStepPin,
            TiltDirPin, TiltHallPin, TiltAddress,
            TiltGR);

Motor:: Motor(String name,
              int steps, int step_pin, int dir_pin, 
              int hall_pin, int eeprom_address, 
              double gr){

  MOTOR_NAME = name;
  STEPS = steps;
  STEP_PIN = step_pin;
  DIR_PIN = dir_pin;
  HALL_PIN = hall_pin;
  GEAR_RATIO = gr;

  // CURR_POSITION = EEPROM.read(eeprom_address); // initalizes position by reading it from EEPROM: https://docs.arduino.cc/learn/built-in-libraries/eeprom
  // if (CURR_POSITION > 180) CURR_POSITION = 0; // zero by default

  CURR_POSITION = 0;
}

void Motor:: init_pin_mode(){
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(HALL_PIN, INPUT);
}

// turns the motors some degrees in specified direction within specified time; timing far from exact, only use as reference
void Motor:: turn(double deg, bool dir, double time){
  if (deg < 1e-3) return ; // exit function if motor doesn't need to turn

  int driver_inc = deg * GEAR_RATIO * (double) STEPS / 360.0;
  // calculate delay

  int delay_per_step = max((time / driver_inc * 5e5), MinMotorDelay);
  // delay per step in microseconds; 1 second = 1e6 microseconds, then divide by 2 since there are two delays to get 5e5
  // 500 microseconds is pretty much the limit; anything faster the motor doesn't pick up


  if (dir == 1) digitalWrite(DIR_PIN, HIGH); // set rotation direction
  else digitalWrite(DIR_PIN, LOW);

  for (int i=0;i<driver_inc;i++){
    digitalWrite(STEP_PIN, HIGH); 
    delayMicroseconds(delay_per_step);     // by changing this time delay between the steps we can change the rotation speed
    digitalWrite(STEP_PIN, LOW); 
    delayMicroseconds(delay_per_step); 
  }
}

// turns the motor to some absolute angular position
void Motor:: turn_to(double angular_position, double time=1){
  if (angular_position < 0 || angular_position > 180){
    Serial.println("OUTSIDE RANGE OF MOTION");
    return ;
  }

  bool dir = (angular_position >= CURR_POSITION ? 1 : 0); // 1 is cw, 0 ccw
  double deg_difference = abs(angular_position - CURR_POSITION);

  this->turn(deg_difference, dir, time);// turn motor
  CURR_POSITION = angular_position;

  Serial.println("Motor " + MOTOR_NAME + " at position " + CURR_POSITION); // debugging
}

// hall effect sensor initalization; usage still uncertain
void Motor:: init_pos(){

}

// converts the degree difference to number of steps taken by the driver motor
int Motor:: deg_to_step(double deg){
  return (deg * GEAR_RATIO / 360.0) * STEPS;
}

void initPanTilt(){
  
  // enable pins for motor (cnc shield); must be at low
  pinMode(MotorEnablePin, OUTPUT);
  digitalWrite(MotorEnablePin, LOW);

  pinMode(PotPin, INPUT);

  pan.init_pin_mode();
  tilt.init_pin_mode();

}

double readSerialInput();

void mainLoop(){

  if (Serial.available()){
    INPUT_STRING = Serial.readStringUntil('\n');
    INPUT_POS = INPUT_STRING.toDouble();
    
    pan.turn_to(INPUT_POS, 0.7);
    tilt.turn_to(INPUT_POS, 0.2);
  }

}

