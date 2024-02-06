#include "slow_control.h"

// Battery voltage parameters
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

// Burning resistor parameters
TCA9534 BR_GPIO;

#define _TCA9534_ADDR 0x20

static int iii = 0;

#define BR_CH1 5
#define BR_CH3 3
#define BR_CH4 0
#define BR_CH6 2

uint8_t PIN_INDEX[BR_GPIO_CNT]      = {BR_CH1, BR_CH3, BR_CH4, BR_CH6};
bool    PIN_MODES[BR_GPIO_CNT]      = {GPIO_OUT, GPIO_OUT, GPIO_OUT, GPIO_OUT};
bool    PIN_INIT_VALUE[BR_GPIO_CNT] = {LOW, LOW, LOW, LOW};

void BR_module_power_on (void) {
  pinMode(BR_DCDC_EN, OUTPUT);
  digitalWrite(BR_DCDC_EN, HIGH);
  delay(100);
}

void BR_GPIO_init(void) {
  BR_module_power_on();

  Wire.begin();

  if (!BR_GPIO.begin(Wire, _TCA9534_ADDR)) {
    if (SERIAL_DEBUG) {
      Serial.println("TCA9534 not found");
    }
    delay(10);
  }

  for (iii = 0; iii < BR_GPIO_CNT; iii ++) {
    BR_GPIO.pinMode(PIN_INDEX[iii], GPIO_OUT);
    BR_GPIO.digitalWrite(PIN_INDEX[iii], PIN_INIT_VALUE[iii]);
  }
}

void BR_TURN_OFF_ALL(void) {
  for (iii = 0; iii < BR_GPIO_CNT; iii ++) {
    BR_GPIO.digitalWrite(PIN_INDEX[iii], LOW);
  }
}

void BR_BURN_R(uint8_t ch_num) {
  BR_GPIO.digitalWrite(ch_num, HIGH);
}
