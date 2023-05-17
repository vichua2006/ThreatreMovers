#include "Arduino.h"
#include "PanTilt.h"
#include <EEPROM.h>

Motor:: Motor(int step, int dir, int hall, int eeprom_address, double gr){
  STEP_PIN = step;
  DIR_PIN = dir;
  HALL_PIN = hall;
  POSITION = EEPROM.read(eeprom_address); // initalizes position by reading it from EEPROM: https://docs.arduino.cc/learn/built-in-libraries/eeprom
  GEAR_RATIO = gr;
}

void Motor:: init_pin_mode(){
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(HALL_PIN, INPUT);
}

// turns the motors some degrees in specified direction within specified time; timing far from exact, only use as reference
void Motor:: turn(double deg, bool dir, double time){
  if (abs(deg - POSITION) < 1e-3) return ; // exit function if motor doesn't need to turn

  // following code need to be adjusted to gear ratio math

  // calculate delay
  // double delay_per_step = max(time / (double) inc * 5e5, (double) 500);
  // delay per step in microseconds; 1 second = 1e6 microseconds, then divide by 2 since there are two delays to get 5e5
  // 500 microseconds is pretty much the limit; anything faster the motor doesn't pick up

  if (dir == 1) digitalWrite(DIR_PIN, HIGH); // set rotation direction
  else digitalWrite(DIR_PIN, LOW);

}

void Motor:: turn_to(){

}

// hall effect sensor initalization; usage still uncertain
void Motor:: init_pos(){

}

void initPanTilt(){
  const int ENABLE_PIN = 8;

  // enable pins for motor; must be at low
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW);
}