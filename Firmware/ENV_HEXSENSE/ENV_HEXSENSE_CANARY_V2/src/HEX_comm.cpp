#include "HEX_comm.h"
#include "HEX_system.h"
#include "HEX_slowcontrol.h"
#include "HEX_sensor_scd30.h"

uint8_t data_buffer[256];
size_t  message_length;
bool    status;

uint32_t seq_num_i = 0;
uint32_t crc_i     = 0;

void lora_setup (void) {
  // TODO: choose different frequency for different regions
  pinMode(LORA_EN_PIN, OUTPUT);
  digitalWrite(LORA_EN_PIN, HIGH);
  delay(500); // FZ: must add a delay to make sure the power is ready.

  LoRa.setPins(PIN_SERIAL_TX, 2, 3);

  if (digitalRead(FREQ_SELECT_PIN) == HIGH) {
    LoRa.begin(LORA_FREQ_US);
  } else {
    LoRa.begin(LORA_FREQ_EU);
  }

  if (SERIAL_DEBUG) {
    Serial.println("lora_setup");
  }
}

void send_data_to_lora(uint8_t* buffer, size_t message_length) {
  if (SERIAL_DEBUG) {
    Serial.println("Sent data to LoRa");
  }

  LoRa.beginPacket();
  LoRa.write(buffer, message_length);
  LoRa.endPacket();
}

void pack_package(void) {
  Env_data_frame env_data_frame = Env_data_frame_init_zero;
  pb_ostream_t   stream         = pb_ostream_from_buffer(data_buffer, sizeof(data_buffer));

  env_data_frame.ID          = HEXSENSE_ID;
  env_data_frame.time_stamp  = millis();
  env_data_frame.seq_num     = seq_num_i;

  env_data_frame.battery_v   = bat_v;
  env_data_frame.temperature = scd_temperature;
  env_data_frame.co2_level   = scd_co2;
  env_data_frame.humidity    = scd_humidity;

  crc_i = env_data_frame.time_stamp + env_data_frame.seq_num + env_data_frame.temperature + env_data_frame.humidity + env_data_frame.co2_level;
  env_data_frame.crc         = crc_i;

  status = pb_encode(&stream, Env_data_frame_fields, &env_data_frame);
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

void lora_sleep(void) {
  if (SERIAL_DEBUG) {
    Serial.println("lora_sleep");
  }
  LoRa.sleep();
}

void lora_idle(void) {
  if (SERIAL_DEBUG) {
    Serial.println("lora_idle");
  }
  LoRa.idle();
}