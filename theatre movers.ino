#include "MovingHead.h"
#include <DMXSerial.h>

/**************
 * PAN CONFIG *
 **************/
const int PanSteps = 200;
const int PanStepPin = 2;
const int PanDirPin = 5;
const int PanHallPin = 0;
const double PanGR = 144.0 / 17.0;
const double PanUpper = 359.0;
const double PanLower = -359.0;

/***************
 * TILT CONFIG *
 ***************/
const int TiltSteps = 200;
const int TiltStepPin = 3;
const int TiltDirPin = 6;
const int TiltHallPin = 4;
const double TiltGR = 64.0 / 21.0;
const double TiltUpper = 90.0;
const double TiltLower = 0.0;

const int StepperEnablePin = 8; // enable pins for motor (cnc shield); must be at low
const int PotPin = A0;          // pin for potentiometer

const int MinStepperDelay = 0;
const int FixedStepperDelay = 700;

const int PanTiltSegment = 1000;

String INPUT_STRING;
double INPUT_POS;

/**************
 * DMX CONFIG *
 **************/
const int NumberOfChannels = 7;
const int StartChannel = 1;

// create global instances
StepperPins PanPins(PanStepPin, PanDirPin, PanHallPin, StepperEnablePin);
StepperProperty PanProp(PanSteps, PanGR, PanUpper, PanLower);
StepperMotor pan("Pan stepper", PanPins, PanProp);

StepperPins TiltPins(TiltStepPin, TiltDirPin, TiltHallPin, StepperEnablePin);
StepperProperty TiltProp(TiltSteps, TiltGR, TiltUpper, TiltLower);
StepperMotor tilt("Tilt stepper", TiltPins, TiltProp);

DualStepper pan_tilt(pan, tilt);

DualHallSensors pan_tilt_sensors(PanHallPin, TiltHallPin);

MovingHead mover(pan_tilt, pan_tilt_sensors);

double X[] = {32, 128, 0, 333, 270, 34, 52, 126, 123, 6}, Y[] = {89, 0, 45, 4, 89, 8, 68, 90, 23, 15};

int read_dmx(int channel)
{
  return DMXSerial.read(channel);
}

int read_and_map_dmx(int channel, int minimum, int maximum)
{
  return map(read_dmx(channel), 0, 255, minimum, maximum);
}

void apply_dmx()
{
  mover.move_to(read_and_map_dmx(1, 0, 359), read_and_map_dmx(2, 0, 90), FixedStepperDelay);
}

void reset()
{
  mover.move_to(0, 0, FixedStepperDelay);
}

void setup()
{
  Serial.begin(9600);
  mover.init_pin_mode();
  DMXSerial.init(DMXReceiver);
}

void loop()
{
  // while (!Serial.available());
  // INPUT_STRING = Serial.readStringUntil('\n');
  // if (INPUT_STRING == "r"){
  //   reset();
  // } else {
  //   int a = INPUT_STRING.toInt();
  //   mover.move_to(X[a - 1], Y[a - 1], FixedStepperDelay);
  // }

  // delay(10000);

  // for (int i=0;i<10;i++){
  //   mover.move_to(X[i], Y[i], FixedStepperDelay);
  //   delay(1000);
  // }

  // reset();
  // delay(5000);
}
