#include <Arduino.h>

#include "HEX_system.h"
#include "HEX_camera.h"
#include "HEX_comm.h"

void setup() {
  serialport_setup();
  camera_setup();
  lora_setup();
  // capture_photo();
  // get_photo();
}

void loop() {
  // TODO: check receiving command
  if (LoRa.available()) {
    int recv_len = LoRa.available();

      output_debug_info_int("Received length: ", recv_len);

      get_recv_cmd(recv_len);
      handle_cmd(&cam_hex_canary_recv_cmd);
  }

  // delay(1000);
}
