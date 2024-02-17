#include <Arduino.h>

#include "HEX_system.h"
#include "HEX_camera.h"
#include "HEX_comm.h"
#include "HEX_FRAM.h"

void setup()
{
  serialport_setup();
  // lora_setup();
  fram_setup();
  camera_init();
 
  take_photo_from(CAM_1);
  take_photo_from(CAM_6);
}

void loop()
{
  output_debug_info("Looping...");

  while (1)
  {
    ;
  }
  
  // if (LoRa.available())
  // {
  //   int recv_len = LoRa.available();

  //   output_debug_info_int("Received length: ", recv_len);

  //   get_recv_cmd(recv_len);
  //   handle_cmd(&cam_hex_canary_recv_cmd);
  // }
}
