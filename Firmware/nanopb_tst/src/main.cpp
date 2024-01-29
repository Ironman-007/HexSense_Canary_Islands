#include <Arduino.h>

#include <pb_encode.h>
#include <pb_decode.h>

#include "env_comm.pb.h"

uint8_t buffer[256];
size_t  message_length;
bool    status;

float step = 0.0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
}

void decodeReceivedData(const uint8_t* data, size_t size) {
  /* Allocate space for the decoded message. */
  Env_data_frame env_data_frame_recv  = Env_data_frame_init_zero;
  pb_istream_t   stream_recv          = pb_istream_from_buffer(data, size);

  for (unsigned int i = 0; i < size; i++) {
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");

  /* Now we are ready to decode the message. */
  bool status = pb_decode(&stream_recv, Env_data_frame_fields, &env_data_frame_recv);

  /* Check for errors... */
  if (!status){
    Serial.println("Decoding failed");
  }

  /* Print the data contained in the message. */
  Serial.print("temperature: ");
  Serial.println(env_data_frame_recv.temperature);
  Serial.print("humidity: ");
  Serial.println(env_data_frame_recv.humidity);
  Serial.print("co2_level: ");
  Serial.println(env_data_frame_recv.co2_level);
}

void loop() {
  Env_data_frame env_data_frame = Env_data_frame_init_zero;
  pb_ostream_t   stream         = pb_ostream_from_buffer(buffer, sizeof(buffer));

  // put your main code here, to run repeatedly:
  env_data_frame.temperature     = 25.0   + step;
  env_data_frame.humidity        = 50.0   + step;
  env_data_frame.co2_level       = 1000.0 + step;

  status = pb_encode(&stream, Env_data_frame_fields, &env_data_frame);
  message_length = stream.bytes_written;

  Serial.print(status);
  Serial.print(" ");
  Serial.println(message_length);

  step += 0.1;

  decodeReceivedData(buffer, message_length);

  delay(1000);
}
