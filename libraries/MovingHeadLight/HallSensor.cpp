/* source file for pan tilt mechanism
 * Created by: Victor Huang and Rohan Katreddy
 * June 22, 2023
 */

#include "HallSensor.h"

DualHallSensors:: HallSensor:: HallSensor(int pin){
  PIN = pin;
}

void DualHallSensors:: HallSensor:: init_pin_mode(){
  pinMode(PIN, INPUT);
}

int DualHallSensors:: HallSensor:: read(){
  return digitalRead(this->PIN);
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

void DualHallSensors:: init_pin_mode(){
  panHall.init_pin_mode();
  tiltHall.init_pin_mode();
}

int DualHallSensors:: read_pan_hall(){
  return panHall.read();
}

int DualHallSensors:: read_tilt_hall(){
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

