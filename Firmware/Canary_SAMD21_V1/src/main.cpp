#include <Arduino.h>

// #include "LowPower.h"
#include "RTC_FZ.h"

#if !( defined(ARDUINO_SAMD_ZERO) || defined(__SAMD21G18A__))
  #error This code is designed to run on SAMD21/SAMD51 platform! Please check your Tools->Board setting.
#endif

#define PERIOD 10 // seconds

RTCZero rtc;

enum low_power_t {
  LOW_POWER_IDLE0,
  LOW_POWER_IDLE1,
  LOW_POWER_IDLE2,
  LOW_POWER_STANDBY
};

#define LOW_POWER_LEVEL LOW_POWER_STANDBY

volatile bool alarmMatched = false;

void alarmMatch(void) {
  alarmMatched = true;
}

void blink_led(void) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
}

void set_idle_level(int level) {
  if (level < 3) {
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk; // Clear SLEEPDEEP bit, we use the idle state.

    if (level == 0) {PM->SLEEP.reg = PM_SLEEP_IDLE_CPU;}
    if (level == 1) {PM->SLEEP.reg = PM_SLEEP_IDLE_AHB;}
    if (level == 2) {PM->SLEEP.reg = PM_SLEEP_IDLE_APB;}
  }
  else {
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
  }
  __DSB();
  __WFI();
}

void setup() {
  // set up led pin as an output:
  pinMode(LED_BUILTIN, OUTPUT);

  // https://github.com/IowaDave/SAMD21-RTC-Clock
  // https://blog.thea.codes/understanding-the-sam-d21-clocks/

  rtc.begin(true, 1);              // initialize RTC: reset starting value, Mode 1 (16-bit counter)
  rtc.setPeriod(PERIOD-1);         // set counter period
  rtc.enableOverflow();            // enable interrupt on overflow
  rtc.attachInterrupt(alarmMatch); // attach interrupt handler
}

void loop() {
  set_idle_level(LOW_POWER_LEVEL);

  if (alarmMatched) {
    alarmMatched = false;
    blink_led();
    set_idle_level(2);
  }
}

