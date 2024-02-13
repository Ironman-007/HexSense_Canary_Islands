#include <Arduino.h>

#include "HEX_system.h"
#include "HEX_camera.h"

void setup() {
  serialport_setup();
  camera_setup();
  capture_photo();
  get_photo();
}

void loop() {
  delay(1000);
}
