#include "slow_control.h"
#include "system.h"

float bat_v = 0.0;

void read_bat_v(uint8_t bat_v_pin) {
  float _batv = analogRead(bat_v_pin);
  bat_v = (_batv -_BAT_V_B) / _BAT_V_A;
}

bool check_5v(uint8_t _5v_pin) {
  uint32_t _5v = analogRead(_5v_pin);

  if (SERIAL_DEBUG) {
    Serial.print("5V: ");
    Serial.println(_5v);
  }

  if (_5v > _5V_THRESHOLD) {
    return true;
  } else {
    return false;
  }
}