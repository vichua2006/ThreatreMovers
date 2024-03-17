#include <Arduino.h>
#include <AccelStepper.h>
#include <DmxInput.h>
#include <PicoConfig.h>
#include <PicoMover.h>

#define LEDPIN 25
#define START_CHANNEL 1
#define NUM_CHANNELS 6


AccelStepper pan(AccelStepper::DRIVER, PanStepPin, PanDirPin);
AccelStepper tilt(AccelStepper::DRIVER, TiltStepPin, TiltDirPin);
Mover mover;

const double tilt_adj = (double) TiltSteps / 2.0 * TiltGR / 255.0;
const double pan_adj = (double) PanSteps * PanGR / 255.0;

DmxInput dmx_input;
volatile uint8_t buffer[DMXINPUT_BUFFER_SIZE(START_CHANNEL, NUM_CHANNELS)];


void setup(){
    Serial.begin(9600);
    
    // set enable pin for stepper drivers; must be at low
    pinMode(LEDPIN, OUTPUT);
    pinMode(StepperEnablePin, OUTPUT);
    digitalWrite(StepperEnablePin, LOW);

    // control pin controls I/O mode of rs485 module; must be set to low for input;
    pinMode(DMXControlPin, OUTPUT);
    digitalWrite(DMXControlPin, LOW);

    // add steppers and set speed and acceleration
    mover.add_steppers(pan, tilt);
    mover.set_max_speed(PanMaxSpeed, TiltMaxSpeed);
    mover.set_accel(AccelFactor * PanMaxSpeed, AccelFactor * TiltMaxSpeed);

}

void loop(){
    // mover.coordinate(buffer[5] * pan_adj, buffer[6] * tilt_adj);
    mover.coordinate(0, 500);
    while(mover.run_towards());
    mover.coordinate(0, 0);
    while(mover.run_towards());
    // tilt.moveTo(1000);

    // Serial.println(buffer[5]);
}

void setup1(){
    dmx_input.begin(DMXReceiverPin, START_CHANNEL, NUM_CHANNELS);
    dmx_input.read_async(buffer);

}


