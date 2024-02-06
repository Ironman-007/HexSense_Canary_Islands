#include <Arduino.h>

#include "comm.h"
#include "ant_motor.h"
#include "system.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial);

  motor_setup();
  ble_setup();
  startAdv();
}

void loop() {
  if (bleuart.available()) {
    int recv_len = bleuart.available();
    output_debug_info_int("Received length: ", recv_len);
  }
}
