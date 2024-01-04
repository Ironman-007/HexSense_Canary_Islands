#include <Arduino.h>

#include "RTC_FZ.h"

#if !( defined(ARDUINO_SAMD_ZERO) || defined(__SAMD21G18A__))
  #error This code is designed to run on SAMD21/SAMD51 platform! Please check your Tools->Board setting.
#endif

RTCZero rtc;

volatile bool alarmMatched = false;

void alarmMatch(void) {
  alarmMatched = true;
}

void blink_led(void) {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void setup() {
  // set up led pin as an output:
  pinMode(LED_BUILTIN, OUTPUT);

  // https://github.com/IowaDave/SAMD21-RTC-Clock
  // https://blog.thea.codes/understanding-the-sam-d21-clocks/

  rtc.begin(true, 1); // initialize RTC: reset starting value, Mode 1 (16-bit counter)
  rtc.setPeriod(0); // set counter period
  rtc.enableOverflow(); // enable interrupt on overflow
  rtc.attachInterrupt(alarmMatch); // attach interrupt
}

void loop() {
  if (alarmMatched) {
    alarmMatched = false;
    blink_led();
  }
}
