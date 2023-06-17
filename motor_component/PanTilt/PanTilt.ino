#include "PanTilt.h"

void setup() {
  Serial.begin(9600);
  initPanTilt();
}

void loop() {
  mainLoop();
}
