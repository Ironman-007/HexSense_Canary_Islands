#include <Arduino.h>

#include "HEX_IMU.h"

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  imu_setup();
}

void loop() {
  read_imu();

  delay(1000);
}
