#include "MovingHead.h"
#include "GeneralFunc.h"
#include <config.h>


#ifdef USE_DMX
#include <DMXSerial.h>
#endif


// create class instances and build mover object
StepperPins PanPins(PanStepPin, PanDirPin, PanHallPin, StepperEnablePin);
StepperProperty PanProp(PanSteps, PanGR, PanUpper, PanLower);
StepperMotor pan("Pan stepper", PanPins, PanProp);

StepperPins TiltPins(TiltStepPin, TiltDirPin, TiltHallPin, StepperEnablePin);
StepperProperty TiltProp(TiltSteps, TiltGR, TiltUpper, TiltLower);
StepperMotor tilt("Tilt stepper", TiltPins, TiltProp);

DualStepper pan_tilt(pan, tilt);

HallSensor pan_sensor(PanHallPin), tilt_sensor(TiltHallPin);
DualHallSensors pan_tilt_sensors(pan_sensor, tilt_sensor);

MovingHead mover(pan_tilt, pan_tilt_sensors, DefaultStartChannel);

// double X[] = {32, 128, 0, 333, 270, 34, 52, 126, 123, 6}, Y[] = {89, 0, 45, 4, 89, 8, 68, 90, 23, 15};
// int x = 45, y = 90; // debugging

void setup() {
  #ifndef USE_DMX
  Serial.begin(9600);
  #endif

  mover.init_pin_mode();
}

void loop() {
  mover.main_cycle();

  // while (!Serial.available());
  // String str = Serial.readStringUntil('\n');
  // double pos = str.toDouble();

  // mover.move_to(0, pos);
}


