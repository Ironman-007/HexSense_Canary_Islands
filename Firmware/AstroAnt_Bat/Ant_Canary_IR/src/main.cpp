#include <Arduino.h>
// #include <Adafruit_TinyUSB.h>

#include "NRF52TimerInterrupt.h"
#include "NRF52_ISR_Timer.h"

#include "system.h"
#include "ant_motor.h"
#include "ant_comm.h"

// ===================== interrupt parameter =====================
bool volatile timer4Interrupt_100ms = false;

NRF52Timer ITimer(NRF_TIMER_1);
NRF52_ISR_Timer ISR_Timer;

// ===================== Interrupt functions =====================
void TimerHandler() {
  ISR_Timer.run();
}

void timer_handler_100ms() {
  timer4Interrupt_100ms = true;
}

void interrupt_setup() {
  ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler);
  ISR_Timer.setInterval(TIMER_INTERVAL_100ms, timer_handler_100ms);
}

void setup() {
  Serialsetup();
  motor_setup();
  interrupt_setup();

  ble_setup();

  startAdv();

  // motor_tst(200);
}

void loop() {
  ant_comm_update();

  // if (timer4Interrupt_100ms) {
  //   timer4Interrupt_100ms = false;
  //   Serial.print("EN_1:");
  //   Serial.print(encoder1Counter/256);
  //   Serial.print(",");
  //   Serial.print("EN_2:");
  //   Serial.println(encoder2Counter/256);
  // }
}
