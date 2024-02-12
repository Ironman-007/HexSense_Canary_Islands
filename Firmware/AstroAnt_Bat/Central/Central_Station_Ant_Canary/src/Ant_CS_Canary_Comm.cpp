#include "Ant_CS_Canary_Comm.h"

uint8_t cmd_buffer[20] = {0};

void send_cmd(CMD_RECV cmd2send) {
  Ant_cmd_frame cmd_frame = Ant_cmd_frame_init_zero;
  pb_ostream_t stream     = pb_ostream_from_buffer(cmd_buffer, sizeof(cmd_buffer));

  cmd_frame.cmd_recv = cmd2send;
  cmd_frame.dis_ang = 10;
  cmd_frame.crc = cmd_frame.dis_ang + cmd_frame.cmd_recv;

  pb_encode(&stream, Ant_cmd_frame_fields, &cmd_frame);
  size_t message_length = stream.bytes_written;

  Serial.print("message_length: ");
  Serial.println(message_length);
  // print the whole data_buffer
  for (size_t i = 0; i < message_length; i++) {
    Serial.print(cmd_buffer[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void print_recv_data(void) {
  ;
}