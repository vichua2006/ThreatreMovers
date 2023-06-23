#include "MovingHead.h"

const int PanSteps = 200;
const int PanStepPin = 2;
const int PanDirPin = 5;
const int PanHallPin = 0;
const double PanGR = 144 / 17;
const double PanUpper = 359.0;
const double PanLower = -359.0;

const int TiltSteps = 200;
const int TiltStepPin = 3;
const int TiltDirPin = 6;
const int TiltHallPin = 0;
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

// create global instances
StepperPins PanPins(PanStepPin, PanDirPin, PanHallPin, MotorEnablePin);
StepperProperty PanProp(PanSteps, PanGR, PanUpper, PanLower);
StepperMotor pan("Pan stepper", PanPins, PanProp);

StepperPins TiltPins(TiltStepPin, TiltDirPin, TiltHallPin, MotorEnablePin);
StepperProperty TiltProp(TiltSteps, TiltGR, TiltUpper, TiltLower);
StepperMotor tilt("Tilt stepper", TiltPins, TiltProp);

DualStepper pan_tilt(pan, tilt);

DualHallSensors pan_tilt_sensors(PanHallPin, TiltHallPin);

MovingHead mover(pan_tilt, pan_tilt_sensors);

void setup() {
  Serial.begin(9600);
  mover.init_pin_mode();
}

void loop() {
  if (Serial.available()){
    INPUT_STRING = Serial.readStringUntil('\n');
    INPUT_POS = INPUT_STRING.toDouble();

    mover.move_to(INPUT_POS, INPUT_POS / 4, FixedStepperDelay);
  }
}
