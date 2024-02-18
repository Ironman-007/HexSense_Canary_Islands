#include "HEX_system.h"

void serialport_setup(void) {
  if (SERIAL_DEBUG) {
    Serial.begin(115200);
    while (!Serial) {
      delay(10);
    }

    Serial.println("Starting HexSense system...");
  }
}

void output_debug_info(const char *info) {
  if (SERIAL_DEBUG) Serial.println(info);
}

void output_debug_info_int(const char *info, int value) {
  if (SERIAL_DEBUG) {
    Serial.print(info);
    Serial.println(value);
  }
}

void output_debug_info_float(const char *info, float value) {
  if (SERIAL_DEBUG) {
    Serial.print(info);
    Serial.println(value);
  }
}

void output_debug_info_int32(const char *info, uint32_t value) {
  if (SERIAL_DEBUG) {
    Serial.print(info);
    Serial.println(value);
  }
}