#include <Arduino.h>

#include "HEX_system.h"
#include "HEX_slowcontrol.h"
#include "HEX_IMU.h"
#include "HEX_sensor_scd30.h"
#include "HEX_comm.h"

#define WAIT_TIME 30000

void system_init(void) {
  if (SERIAL_DEBUG) serial_setup();

  if (!SERIAL_DEBUG) delay(WAIT_TIME);

  // system_pin_init();
  imu_setup();
  RTC_setup();
  lora_setup();
  // sensor_scd30_setup();
  // BR_GPIO_init();
  // BR_TURN_OFF_ALL();

  // flash_led(50, 3);

  // calculate_orientation();

  // if (SERIAL_DEBUG) {
  //   Serial.print("Orientation: ");
  //   Serial.println(body_orientation);
  // }
  // led_on();
  // Burn_resistor(body_orientation);
  // led_off();
}

void setup(void) {
  if (SERIAL_DEBUG) {
    system_init();
    Serial.println("System init done");
  }
  else {
    while (check_5v()) { // if 5V exist, then do nothing.
      delay(100);
    }
    system_init();
  }  
}

void loop() {

  if (alarmMatched) {
    alarmMatched = false;

    lora_idle();
    read_bat_v();

    pack_package();

    // if (read_scd30()) {
    //   if (SERIAL_DEBUG) {
    //     Serial.println("Send data");
    //   }
    //   pack_package();
    //   flash_led(100, 1);
    // }
    lora_sleep();
    set_idle_level(LOW_POWER_LEVEL);
  }
}
