#include "system.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

void Serialsetup(void) {
  if (SERIAL_DEBUG) {
    Serial.begin(115200);

    while (!Serial) {
      delay(10);
    }
  }

  output_debug_info("Serial setup done");
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

uint32_t read_bat_V(void) {
 return analogRead(BAT_V_PIN);
}

#ifdef __cplusplus
}
#endif