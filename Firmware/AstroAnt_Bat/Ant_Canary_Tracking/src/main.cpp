// protoc --nanopb_out=. ant_canary.proto

#include <Arduino.h>

#include "comm.h"
#include "ant_motor.h"
#include "system.h"

void setup() {
  // put your setup code here, to run once:
  Serialsetup();
  system_setup();

  motor_setup();
  ble_setup();
  startAdv();
}

void loop() {
  if (bleConnected) {
    if (bleuart.available()) {
      int recv_len = bleuart.available();
      output_debug_info_int("Received length: ", recv_len);
      _CMD_RECV cmd_recv = get_recv_cmd(recv_len);
      // handle_cmd(cmd_recv);
    }
  } else {
    startAdv();
  }
}


