// protoc --nanopb_out=. ant_canary.proto

#include <Arduino.h>

#include "comm.h"
#include "ant_motor.h"
#include "system.h"
#include "ant_canary_tracking.h"

#include "NRF52TimerInterrupt.h"
#include "NRF52_ISR_Timer.h"

volatile bool timer4Interrupt_100ms = false;

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
  // put your setup code here, to run once:
  Serialsetup();
  system_setup();

  IMU_setup();
  motor_setup();
  ble_setup();
  interrupt_setup();

  startAdv();
}

void loop() {
  if (bleConnected) {
    if (bleuart.available()) {
      int recv_len = bleuart.available();

      output_debug_info_int("Received length: ", recv_len);

      get_recv_cmd(recv_len);
      handle_cmd(&ant_canary_recv_cmd);
    }
  } else {
    startAdv();
  }
}

