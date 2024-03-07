#include <AccelStepper.h>
#include "MovingHead.h"
#include "GeneralFunc.h"
#include <config.h>


// to avoid conflict with serial in/out
#ifdef USING_DMX
#include <DMXSerial.h>
#endif


AccelStepper pan(AccelStepper:: DRIVER, PanStepPin, PanDirPin);
AccelStepper tilt(AccelStepper:: DRIVER, TiltStepPin, TiltDirPin);


DualStepper pan_tilt(pan, tilt);

HallSensor pan_sensor(PanHallPin), tilt_sensor(TiltHallPin);
DualHallSensors pan_tilt_sensors(pan_sensor, tilt_sensor);

MovingHead mover(pan_tilt, pan_tilt_sensors, DefaultStartChannel);

// double X[] = {32, 128, 0, 333, 270, 34, 52, 126, 123, 6}, Y[] = {89, 0, 45, 4, 89, 8, 68, 90, 23, 15};
// int x = 45, y = 90; // debugging

void setup() {
  #ifndef USING_DMX
  Serial.begin(9600);
  #endif

  mover.init_pin_mode();
  mover.set_speed(PanMaxSpeed, TiltMaxSpeed);
  mover.set_accel(2 * PanMaxSpeed, 2 * TiltMaxSpeed);
  
}

void loop() {
  mover.main_cycle();

  // pan_tilt.step_towards(90, 90);

  // while (!Serial.available());
  // String str = Serial.readStringUntil('\n');
  // double pos = str.toDouble();

  // mover.move_to(0, pos);
}

