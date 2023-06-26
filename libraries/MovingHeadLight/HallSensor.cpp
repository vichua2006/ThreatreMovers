/* source file for pan tilt mechanism
 * Created by: Victor Huang and Rohan Katreddy
 * June 22, 2023
 */

#include "HallSensor.h"
#include <Arduino.h>

HallSensor:: HallSensor(int pin){
  PIN = pin;
}

void HallSensor:: init_pin_mode(){
  pinMode(PIN, INPUT);
}

int HallSensor:: get_pin(){
  return PIN;
}

int HallSensor:: read(){
  return digitalRead(PIN);
}

DualHallSensors:: DualHallSensors(HallSensor& PanH, HallSensor& TiltH) : panHall(PanH), tiltHall(TiltH){
  
}

void DualHallSensors:: init_pin_mode(){
  panHall.init_pin_mode();
  tiltHall.init_pin_mode();

  // initialise interrupts
  attachInterrupt(digitalPinToInterrupt(panHall.get_pin()), pan_hall_changingISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(tiltHall.get_pin()), tilt_hall_changingISR, CHANGE);
}

int DualHallSensors:: read_pan_hall(){
  return panHall.read();
}

int DualHallSensors:: read_tilt_hall(){
  return tiltHall.read();
}

void DualHallSensors:: pan_hall_changingISR(){
  isPanHallClosed ^= 1;
}

void DualHallSensors:: tilt_hall_changingISR(){
  isTiltHallClosed ^= 1;
}

bool DualHallSensors:: isPanHallClosed = false;
bool DualHallSensors:: isTiltHallClosed = false;

