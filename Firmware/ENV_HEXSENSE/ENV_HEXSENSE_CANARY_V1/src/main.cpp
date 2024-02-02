#include <Arduino.h>

#include "system.h"
#include "slow_control.h"
#include "sensor_scd30.h"
#include "comm.h"

void setup () {
  if (SERIAL_DEBUG) {
    Serial.begin(115200);
    while (!Serial) {
      delay(10);
    }
  }

  if (SERIAL_DEBUG) {
    Serial.println("Start");
  }

  RTC_setup();
  lora_setup();
  sensor_scd30_setup();
}

void loop () {
  if (alarmMatched) {
    alarmMatched = false;

    read_bat_v();

    if (read_scd30()) {
      if (SERIAL_DEBUG) {
        Serial.println("Send data");
      }
      pack_package();
    }
    set_idle_level(LOW_POWER_LEVEL);
  }
}
