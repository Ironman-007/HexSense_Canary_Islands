#pragma once

#include <Arduino.h>

#include <pb_encode.h>
#include <pb_decode.h>

#include <SPI.h>
#include <LoRa.h>

#include "cam_hex_canary.pb.h"
#include "HEX_system.h"

#define LORA_EN_PIN  A3
#define LORA_CS_PIN  PIN_SERIAL_TX
#define LORA_IRQ_PIN 4

#define HEXSENSE_ID 30

#define FREQ_SELECT_PIN 10

#define LORA_FREQ_US 915E6

extern void lora_setup(void);
extern void pack_package(void);

extern void lora_sleep(void);
extern void lora_idle(void);

extern uint8_t cmd_buffer[50];

class CAM_HEX_CANARY_RECV_CMD {
  public:
    CAM_HEX_CANARY_RECV_CMD(_CMD_RECV cmd) : recv_cmd(cmd) {}

    _CMD_RECV recv_cmd;
};

extern int get_recv_cmd(size_t size);
extern void handle_cmd(CAM_HEX_CANARY_RECV_CMD * cmd_recv);

extern CAM_HEX_CANARY_RECV_CMD cam_hex_canary_recv_cmd;

