#include "HEX_slowcontrol.h"
#include "HEX_IMU.h"

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

#define BR_CH1 5 // SIDE 1
#define BR_CH3 3 // SIDE 2
#define BR_CH6 2 // SIDE 4
#define BR_CH4 0 // SIDE 5

uint8_t PIN_INDEX[BR_GPIO_CNT]      = {BR_CH1, BR_CH3, BR_CH4, BR_CH6};
// bool    PIN_MODES[BR_GPIO_CNT]      = {GPIO_OUT, GPIO_OUT, GPIO_OUT, GPIO_OUT};
// bool    PIN_INIT_VALUE[BR_GPIO_CNT] = {LOW, LOW, LOW, LOW};

void BR_module_power_on (void) {
  pinMode(BR_DCDC_EN, OUTPUT);
  digitalWrite(BR_DCDC_EN, HIGH);
  delay(100);
}

void BR_GPIO_init(void) {
  BR_module_power_on();

  Wire.begin();
  BR_GPIO.begin(Wire, _TCA9534_ADDR);

  // if (!BR_GPIO.begin(Wire, _TCA9534_ADDR)) {
  //   if (SERIAL_DEBUG) {
  //     Serial.println("TCA9534 not found");
  //   }
  //   delay(10);
  // }

  for (iii = 0; iii < BR_GPIO_CNT; iii ++) {
    BR_GPIO.pinMode(PIN_INDEX[iii], GPIO_OUT);
    BR_GPIO.digitalWrite(PIN_INDEX[iii], LOW);
  }
}

void BR_TURN_OFF_ALL(void) {
  for (iii = 0; iii < BR_GPIO_CNT; iii ++) {
    BR_GPIO.digitalWrite(PIN_INDEX[iii], LOW);
  }
}

void BR_BURN_R(uint8_t ch_num) {
  BR_GPIO.digitalWrite(ch_num, HIGH);
  if (SERIAL_DEBUG) {
    Serial.print("Burning resistor: ");
    Serial.println(ch_num);
  }
}

static void try2burn_R(uint8_t CH_num, int first_try, int second_try) {
  BR_TURN_OFF_ALL();
  BR_BURN_R(CH_num);
  delay(first_try);
  BR_TURN_OFF_ALL();

  // 2s for the node to ba stable before checking if the second burn is necessary
  delay(2000);

  read_imu();

  if (SERIAL_DEBUG) {
    Serial.print("AccZ = ");
    Serial.println(acc_z);
  }

  // check the orientation
  // if didn't stand up, reburn
  if (acc_z < STANDING_UP_TRESHOLD) {
    if (SERIAL_DEBUG) {
      Serial.println("Try to burn again");
    }
    BR_BURN_R(CH_num);
    delay(second_try);
    BR_TURN_OFF_ALL();
  }
}

void Burn_resistor(float orientation) {
  if (orientation >= 0 && orientation < 90) {
    try2burn_R(BR_CH1, BURNING_PERIODE, BURNING_PERIODE);
    // BR_BURN_R(BR_CH1);
  }
  if (orientation >= 90 && orientation < 180) {
    try2burn_R(BR_CH3, BURNING_PERIODE, BURNING_PERIODE);
    // BR_BURN_R(BR_CH3);
  }
  if (orientation >= 180 && orientation < 270) {
    try2burn_R(BR_CH6, BURNING_PERIODE, BURNING_PERIODE);
    // BR_BURN_R(BR_CH6);
  }
  if (orientation >= 270 && orientation < 360) {
    try2burn_R(BR_CH4, BURNING_PERIODE, BURNING_PERIODE);
    // BR_BURN_R(BR_CH4);
  }
}