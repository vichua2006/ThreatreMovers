#include "MovingHead.h"
#include "GeneralFunc.h"
#include <DMXSerial.h>
#include <config.h>

// create global instances
StepperPins PanPins(PanStepPin, PanDirPin, PanHallPin, StepperEnablePin);
StepperProperty PanProp(PanSteps, PanGR, PanUpper, PanLower);
StepperMotor pan("Pan stepper", PanPins, PanProp);

StepperPins TiltPins(TiltStepPin, TiltDirPin, TiltHallPin, StepperEnablePin);
StepperProperty TiltProp(TiltSteps, TiltGR, TiltUpper, TiltLower);
StepperMotor tilt("Tilt stepper", TiltPins, TiltProp);

DualStepper pan_tilt(pan, tilt);

HallSensor pan_sensor(PanHallPin), tilt_sensor(TiltHallPin);
DualHallSensors pan_tilt_sensors(pan_sensor, tilt_sensor);

MovingHead mover(pan_tilt, pan_tilt_sensors);

double X[] = {32, 128, 0, 333, 270, 34, 52, 126, 123, 6}, Y[] = {89, 0, 45, 4, 89, 8, 68, 90, 23, 15};

int x = 45, y = 90;

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

void setup() {
  mover.init_pin_mode();
  DMXSerial.init(DMXReceiver, 5);
}

void loop() {
  apply_dmx();
}



