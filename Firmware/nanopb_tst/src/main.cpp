#include <Arduino.h>

#include <pb_encode.h>
#include <pb_decode.h>

#include "env_comm.pb.h"

uint8_t buffer[256];
size_t  message_length;
bool    status;

float step = 0.0;

uint32_t seq_num_i = 0;
uint32_t crc_i     = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
}

void decodeReceivedData(const uint8_t* data, size_t size) {
  Env_data_frame env_data_frame_recv  = Env_data_frame_init_zero;
  pb_istream_t   stream_recv          = pb_istream_from_buffer(data, size);

  for (unsigned int i = 0; i < size; i++) {
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
  bool status = pb_decode(&stream_recv, Env_data_frame_fields, &env_data_frame_recv);

  if (!status){
    Serial.println("Decoding failed");
  }

  /* Print the data contained in the message. */
  Serial.print("ID: ");
  Serial.println(env_data_frame_recv.ID);
  Serial.print("time_stamp: ");
  Serial.println(env_data_frame_recv.time_stamp);
  Serial.print("seq_num: ");
  Serial.println(env_data_frame_recv.seq_num);
  Serial.print("temperature: ");
  Serial.println(env_data_frame_recv.temperature);
  Serial.print("humidity: ");
  Serial.println(env_data_frame_recv.humidity);
  Serial.print("co2_level: ");
  Serial.println(env_data_frame_recv.co2_level);
  Serial.print("CRC: ");
  Serial.println(env_data_frame_recv.crc);
}

void send2LoRa(uint8_t* buffer, size_t message_length) {
  ;
}

void cobs_encode_msg(uint8_t* buffer, size_t message_length) {
  ;
}

void loop() {
  Env_data_frame env_data_frame = Env_data_frame_init_zero;
  pb_ostream_t   stream         = pb_ostream_from_buffer(buffer, sizeof(buffer));

  env_data_frame.ID          = 10;
  env_data_frame.time_stamp  = millis();
  env_data_frame.seq_num = seq_num_i;
  env_data_frame.temperature = 25.0   + step;
  env_data_frame.humidity    = 50.0   + step;
  env_data_frame.co2_level   = 1000.0 + step;
  crc_i = env_data_frame.time_stamp + env_data_frame.seq_num + env_data_frame.temperature + env_data_frame.humidity + env_data_frame.co2_level;
  env_data_frame.crc         = crc_i;

  status = pb_encode(&stream, Env_data_frame_fields, &env_data_frame);
  message_length = stream.bytes_written;

  Serial.print(status);
  Serial.print(" ");
  Serial.println(message_length);

  step += 0.1;
  seq_num_i++;

  // decodeReceivedData(buffer, message_length);

  delay(2000);
}