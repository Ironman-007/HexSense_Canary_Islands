#pragma once

#pragma once

#include <Arduino.h>
#include <Wire.h>

#include "HEX_system.h"
#include "SparkFun_TCA9534.h"

// Battery voltage parameters
#define _BAT_V_PIN A1
#define _BAT_V_A   146.15
#define _BAT_V_B   28.23

#define _5V_PIN       A0
#define _5V_THRESHOLD 500

extern float bat_v;

extern void read_bat_v(uint8_t bat_v_pin = _BAT_V_PIN);
extern bool  check_5v(uint8_t _5v_pin = _5V_PIN);

// Burning resistor parameters
#define BR_DCDC_EN PIN_SERIAL_RX

#define BR_GPIO_CNT  4

extern TCA9534 BR_GPIO;

#define BURNING_PERIODE 10000
#define STANDING_UP_TRESHOLD 8

// pin_index: channels to be used
extern void BR_GPIO_init(void);
extern void BR_TURN_OFF_ALL(void);
extern void BR_BURN_R(uint8_t ch_num);
extern void Burn_resistor(float orientation);