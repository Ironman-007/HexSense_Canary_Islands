#pragma once

#include <Arduino.h>

#include "RTC_FZ.h"

#define SERIAL_DEBUG true

#if !( defined(ARDUINO_SAMD_ZERO) || defined(__SAMD21G18A__))
  #error This code is designed to run on SAMD21/SAMD51 platform! Please check your Tools->Board setting.
#endif

#define PERIOD 10 // sleep period in seconds

#define INDICATOR_LED_PIN 9
#define FREQ_SELECT_PIN   10

enum low_power_t {
  LOW_POWER_IDLE0,
  LOW_POWER_IDLE1,
  LOW_POWER_IDLE2,
  LOW_POWER_STANDBY
};

#define LOW_POWER_LEVEL   LOW_POWER_IDLE0

extern volatile bool alarmMatched;

extern void flash_led(int period, int times);
extern void led_on(void);
extern void led_off(void);

extern void system_pin_init(void);

extern void serial_setup(void);

extern void alarmMatch(void);
extern void set_idle_level(int level);

extern void RTC_setup(void);

