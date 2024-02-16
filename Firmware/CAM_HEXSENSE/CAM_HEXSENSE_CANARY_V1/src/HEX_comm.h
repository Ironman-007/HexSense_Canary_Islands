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

extern uint8_t data_buffer[256];
extern uint8_t img2send_cobs_encode_buffer[150];

extern CAM_HEX_camera_data_frame_CAM_data_t camera_data;

class CAM_HEX_CANARY_RECV_CMD {
  public:
    CAM_HEX_CANARY_RECV_CMD(_CMD_RECV cmd) : recv_cmd(cmd) {}

    _CMD_RECV recv_cmd;
};

extern int get_recv_cmd(size_t size);
extern void handle_cmd(CAM_HEX_CANARY_RECV_CMD * cmd_recv);

/// \brief Encode a byte buffer with the COBS encoder.
/// \param buffer A pointer to the unencoded buffer to encode.
/// \param size  The number of bytes in the \p buffer.
/// \param encodedBuffer The buffer for the encoded bytes.
/// \returns The number of bytes written to the \p encodedBuffer.
/// \warning The encodedBuffer must have at least getEncodedBufferSize() allocated.
extern size_t COBSencode(uint8_t* buffer, size_t size, uint8_t* encodedBuffer);
extern size_t COBSdecode(uint8_t* encodedBuffer, size_t size, uint8_t* decodedBuffer);

extern CAM_HEX_CANARY_RECV_CMD cam_hex_canary_recv_cmd;

extern void tst_function(void);