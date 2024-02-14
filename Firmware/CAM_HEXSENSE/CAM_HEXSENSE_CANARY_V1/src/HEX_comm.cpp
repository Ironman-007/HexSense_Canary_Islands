#include "HEX_comm.h"
#include "HEX_system.h"
#include "HEX_camera.h"

uint8_t data_buffer[256];
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

int get_recv_cmd(size_t size) {
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

void send_ack(_CMD_RECV cmd_recv, _ACK2SEND ack2send) {
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

void handle_cmd(CAM_HEX_CANARY_RECV_CMD * cmd_recv) {
  seq_num_i = 0;

  if (cmd_recv->recv_cmd == CMD_RECV_INVALID)
  {
    send_ack(CMD_RECV_INVALID, ACK2SEND_NCK);
  }

  if (cmd_recv->recv_cmd == CMD_RECV_START)
  {
    send_ack(CMD_RECV_START, ACK2SEND_ACK);
    // TODO: capture image and send back
  }

  if (cmd_recv->recv_cmd == CMD_RECV_AGAIN)
  {
    send_ack(CMD_RECV_AGAIN, ACK2SEND_ACK);
    // TODO: send the image from the FRAM again
  }
}

void pack_package(void)
{
  CAM_HEX_camera_data_frame camera_data_frame = CAM_HEX_camera_data_frame_init_zero;
  pb_ostream_t   stream                       = pb_ostream_from_buffer(data_buffer, sizeof(data_buffer));

  camera_data_frame.seq_num = seq_num_i;

  // TODO: read data from FRAM and pack into the CAM_data.
  // camera_data_frame.CAM_data = bat_v;

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

void lora_sleep(void)
{
  LoRa.sleep();
}

void lora_idle(void)
{
  LoRa.idle();
}