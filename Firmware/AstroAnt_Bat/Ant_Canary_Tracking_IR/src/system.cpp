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

void system_setup(void) {
  pinMode(BAT_V_PIN, INPUT);

  pinMode(MSG_LED_PIN, OUTPUT);
  digitalWrite(MSG_LED_PIN, LOW);

  pinMode(BLE_LED_PIN, OUTPUT);
  digitalWrite(BLE_LED_PIN, LOW);

  output_debug_info("System setup done");

  flash_led(MSG_LED_PIN, 3, 100);
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

void flash_led(uint8_t pin, uint8_t times, uint16_t interval) {
  for (uint8_t i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(interval);
    digitalWrite(pin, LOW);
    delay(interval);
  }
}

uint32_t read_bat_V(void) {
 return analogRead(BAT_V_PIN);
}

#ifdef __cplusplus
}
#endif