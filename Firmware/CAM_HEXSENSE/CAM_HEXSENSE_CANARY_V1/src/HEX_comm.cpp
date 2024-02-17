#include "HEX_comm.h"
#include "HEX_system.h"
#include "HEX_camera.h"
#include "HEX_FRAM.h"

uint8_t data_buffer[256] = {0};
uint8_t img2send_cobs_encode_buffer[150] = {0};

CAM_HEX_camera_data_frame_CAM_data_t camera_data;

uint8_t img2send_cobs_decode_buffer[100] = {0};

size_t  message_length;
bool    status;

uint32_t seq_num_i = 0;
uint32_t crc_i     = 0;

uint8_t cmd_buffer[50] = {0};

CAM_HEX_CANARY_RECV_CMD cam_hex_canary_recv_cmd(CMD_RECV_INVALID);

void lora_setup (void)
{
  pinMode(LORA_EN_PIN, OUTPUT);
  digitalWrite(LORA_EN_PIN, HIGH);
  delay(100); // FZ: must add a delay to make sure the power is ready.

  LoRa.setPins(PIN_SERIAL_TX, 2, 3);
  LoRa.begin(LORA_FREQ_US);

  output_debug_info("LoRa initialized");
}

void send_data_to_lora(uint8_t* buffer, size_t message_length)
{
  LoRa.beginPacket();
  LoRa.write(buffer, message_length);
  LoRa.endPacket();

  if (SERIAL_DEBUG) {
    Serial.println("Sent data to LoRa");
  }
}

int decodecmd(const uint8_t* data, size_t size) {
  CAM_HEX_cmd_frame CAM_HEX_cmd_frame_recv = CAM_HEX_cmd_frame_init_zero;
  pb_istream_t  stream_recv                = pb_istream_from_buffer(data, size);

  if (SERIAL_DEBUG) {
    Serial.println("Received cmd:");

    for (unsigned int i = 0; i < size; i++) {
      Serial.print(data[i], HEX);
      Serial.print(" ");
    }
  }
  Serial.println("");

  bool status = pb_decode(&stream_recv, CAM_HEX_cmd_frame_fields, &CAM_HEX_cmd_frame_recv);

  if (!status) {
    Serial.println("Decoding failed");
    return 0;
  }

  cam_hex_canary_recv_cmd.recv_cmd = CAM_HEX_cmd_frame_recv.cmd_recv;

  return 1;
}

int get_recv_cmd(size_t size)
{
  for (unsigned int i = 0; i < size; i++)
  {
    cmd_buffer[i] = (uint8_t) LoRa.read();
  }

  decodecmd(cmd_buffer, size);

  if (SERIAL_DEBUG)
  {
    Serial.print("Received cmd:");
    Serial.println(cam_hex_canary_recv_cmd.recv_cmd);
  }

  return 1;
  // return CMD_RECV_INVAID;
}

void send_ack(_CMD_RECV cmd_recv, _ACK2SEND ack2send)
{
  CAM_HEX_ack_frame CAM_HEX_ack_frame_send = CAM_HEX_ack_frame_init_zero;
  pb_ostream_t stream_send = pb_ostream_from_buffer(cmd_buffer, sizeof(cmd_buffer));

  CAM_HEX_ack_frame_send.cmd_recv = cmd_recv;
  CAM_HEX_ack_frame_send.ack2send = ack2send;
  CAM_HEX_ack_frame_send.crc      = cmd_recv + ack2send;

  bool status = pb_encode(&stream_send, CAM_HEX_ack_frame_fields, &CAM_HEX_ack_frame_send);

  if (SERIAL_DEBUG) {
    if (!status) {
      Serial.println("Encoding failed");
      return;
    }
  }

  if (SERIAL_DEBUG) {
    Serial.println("Sending ack:");
    for (unsigned int i = 0; i < stream_send.bytes_written; i++) {
      Serial.print(cmd_buffer[i], HEX);
      Serial.print(" ");
    }
    Serial.println("");
  }

  LoRa.write(cmd_buffer, stream_send.bytes_written);
}

void tst_function(void) {
  capture_photo(); // Take a photo
  get_photo();     // Get the photo in 100 bytes, COBS encode and store it in FRAM

  // while loop, read all data in FRAM
  uint8_t fram_byte = 0x00;
  FRAM_r_P = 0;

  while (FRAM_r_P < FRAM_IMAGE_ADDR_TOP)
  {
    fram_read_byte(FRAM_r_P, &fram_byte);

    if (SERIAL_DEBUG)
    {
      if (fram_byte < 0x10)
      {
        Serial.print("0");
      }
      Serial.print(fram_byte, HEX);
    }

    FRAM_r_P++;
  }
}

void handle_cmd(CAM_HEX_CANARY_RECV_CMD * cmd_recv)
{
  seq_num_i = 0;

  if (cmd_recv->recv_cmd == CMD_RECV_INVALID)
  {
    send_ack(CMD_RECV_INVALID, ACK2SEND_NCK);
  }

  if (cmd_recv->recv_cmd == CMD_RECV_START)
  {
    send_ack(CMD_RECV_START, ACK2SEND_ACK);
    capture_photo(); // Take a photo
    get_photo();     // Get the photo in 100 bytes, COBS encode and store it in FRAM

    // while loop, read all data in FRAM
    uint8_t fram_byte = 0x00;
    FRAM_r_P = 0;

    while (FRAM_r_P < FRAM_w_P)
    {
      while (fram_byte != 0x00)
      {
        fram_read_byte(FRAM_r_P, &fram_byte);
        img2send_cobs_encode_buffer[FRAM_r_P] = fram_byte;
        FRAM_r_P++;
      }

      // COBS decode the image
      size_t img_decode_buffer_size = COBSdecode(img2send_cobs_encode_buffer, FRAM_r_P, camera_data.bytes);

      output_debug_info_int("img_decode_buffer_size: ", img_decode_buffer_size);

      pack_package(); // pack image data into package and send data back through LoRa
    }
  }

  if (cmd_recv->recv_cmd == CMD_RECV_AGAIN)
  {
    send_ack(CMD_RECV_AGAIN, ACK2SEND_ACK);
    // TODO: send the image from the FRAM again
    // pack_package(); 
    // TODO: send data back
  }
}

// void _read_image_from_FRAM(uint8_t* buffer, size_t size)
// {
//   fram_read(0, buffer, size);
// }

void pack_package(void)
{
  CAM_HEX_camera_data_frame camera_data_frame = CAM_HEX_camera_data_frame_init_zero;
  pb_ostream_t   stream                       = pb_ostream_from_buffer(data_buffer, sizeof(data_buffer));

  camera_data_frame.seq_num = seq_num_i;
  camera_data_frame.CAM_data = camera_data;
  camera_data_frame.crc = 0;

  status = pb_encode(&stream, CAM_HEX_camera_data_frame_fields, &camera_data_frame);
  message_length = stream.bytes_written;

  if (SERIAL_DEBUG) {
    Serial.print("message_length: ");
    Serial.println(message_length);
    // print the whole data_buffer
    for (size_t i = 0; i < message_length; i++) {
      Serial.print(data_buffer[i], HEX);
      Serial.print(" ");
    }
  }

  seq_num_i++;

  send_data_to_lora(data_buffer, message_length);
}

size_t COBSencode(uint8_t* buffer, size_t size, uint8_t* encodedBuffer)
{
  size_t  read_index  = 0;
  size_t  write_index = 1;
  size_t  code_index  = 0;
  uint8_t code        = 1;

  while (read_index < size) {
    if (buffer[read_index] == 0) {
      encodedBuffer[code_index] = code;
      code = 1;
      code_index = write_index++;
      read_index++;
    }
    else {
      encodedBuffer[write_index++] = buffer[read_index++];
      code++;

      if (code == 0xFF) {
        encodedBuffer[code_index] = code;
        code = 1;
        code_index = write_index++;
      }
    }
  }

  encodedBuffer[code_index] = code;

  return write_index;
}

/// \brief Decode a COBS-encoded buffer.
/// \param encodedBuffer A pointer to the \p encodedBuffer to decode.
/// \param size The number of bytes in the \p encodedBuffer.
/// \param decodedBuffer The target buffer for the decoded bytes.
/// \returns The number of bytes written to the \p decodedBuffer.
/// \warning decodedBuffer must have a minimum capacity of size.
// size_t decode(const uint8_t* encodedBuffer, size_t size, uint8_t* decodedBuffer)
size_t COBSdecode(uint8_t* encodedBuffer, size_t size, uint8_t* decodedBuffer)
{
  size_t read_index  = 0;
  size_t write_index = 0;
  uint8_t code       = 0;
  uint8_t i          = 0;

  while (read_index < size) {
    code = encodedBuffer[read_index];

    read_index++;

    for (i = 1; i < code; i++) { decodedBuffer[write_index++] = encodedBuffer[read_index++];}

    if (code != 0xFF && read_index != size) {decodedBuffer[write_index++] = '\0';}
  }

  return write_index;
}

void lora_sleep(void)
{
  LoRa.sleep();
}

void lora_idle(void)
{
  LoRa.idle();
}