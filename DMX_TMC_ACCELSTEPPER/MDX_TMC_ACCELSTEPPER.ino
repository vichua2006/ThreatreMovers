#include <Arduino.h>
#include "DmxInput.h"
#include <AccelStepper.h>
#include <TMC2209.h>


DmxInput dmxInput;
AccelStepper tilt(1, 4,3);

#define START_CHANNEL 1
#define NUM_CHANNELS 7
long long m = 0;


volatile uint8_t buffer[DMXINPUT_BUFFER_SIZE(START_CHANNEL, NUM_CHANNELS)];
long long start;

HardwareSerial & serial_stream = Serial2;
uint8_t microsteps_per_step_exponent = 0;
const uint8_t RUN_CURRENT_PERCENT = 50;
const TMC2209::CurrentIncrement CURRENT_INCREMENT = TMC2209::CURRENT_INCREMENT_8;
const TMC2209::MeasurementCount MEASUREMENT_COUNT = TMC2209::MEASUREMENT_COUNT_1;
const uint32_t COOL_STEP_DURATION_THRESHOLD = 2000;
const uint8_t COOL_STEP_LOWER_THRESHOLD = 2;
const uint8_t COOL_STEP_UPPER_THRESHOLD = 0;

TMC2209 stepper_driver;

void setup() {
  Serial.begin(9600);
  dmxInput.begin(0, START_CHANNEL, NUM_CHANNELS);
  dmxInput.read_async(buffer);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);

  Serial2.setTX(8);
  Serial2.setRX(9);
  stepper_driver.setup(serial_stream);
  stepper_driver.setMicrostepsPerStepPowerOfTwo(microsteps_per_step_exponent);
  stepper_driver.setRunCurrent(RUN_CURRENT_PERCENT);
    stepper_driver.enableAutomaticCurrentScaling();
      stepper_driver.enableAutomaticGradientAdaptation();
  stepper_driver.enable();

  tilt.setMaxSpeed(6000);
  tilt.setAcceleration(10000);
   stepper_driver.setCoolStepCurrentIncrement(CURRENT_INCREMENT);
  stepper_driver.setCoolStepMeasurementCount(MEASUREMENT_COUNT);
  stepper_driver.setCoolStepDurationThreshold(COOL_STEP_DURATION_THRESHOLD);
  stepper_driver.enableCoolStep();
  stepper_driver.disableStealthChop();
  stepper_driver.setStandstillMode(stepper_driver.BRAKING);
}

void loop() {
  start = millis();
  long long r = millis()-start;
  m = max(m, millis() - dmxInput.latest_packet_timestamp());
  Serial.println(m);
  tilt.moveTo(map(buffer[1], 0, 255, 0, 400));
  Serial.print("getMicrostepsPerStep() = ");
  Serial.println(stepper_driver.getMicrostepsPerStep());
  if (Serial.available()) {
    int input = Serial.parseInt();
    if (input <= 8){
    stepper_driver.setMicrostepsPerStepPowerOfTwo(input);
    }
    else{
        tilt.setMaxSpeed(input);
        tilt.setAcceleration(input*1.5);
    }
  }

}

void setup1(){

}

void loop1(){
  tilt.run();
}
