#include <Arduino.h>

#include "HEX_system.h"
#include "HEX_slowcontrol.h"
#include "HEX_IMU.h"

void system_init(void) {
  if (SERIAL_DEBUG) serial_setup();

  delay(10000);

  imu_setup();
  BR_GPIO_init();
  BR_TURN_OFF_ALL();

  calculate_orientation();

  if (SERIAL_DEBUG) {
    Serial.print("Orientation: ");
    Serial.println(body_orientation);
  }
  Burn_resistor(body_orientation);
}

void setup(void) {
  system_init();
}

void loop() {
  // if (SERIAL_DEBUG) {
  //   Serial.print("Orientation: ");
  //   calculate_orientation();
  //   Serial.println(body_orientation);
  // }

  delay(2000);
}
