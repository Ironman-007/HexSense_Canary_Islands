#pragma once

#include <Arduino.h>
#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20948.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define _BAT_V_PIN A1
#define _BAT_V_A   146.15
#define _BAT_V_B   28.23

#define _5V_PIN       A0
#define _5V_THRESHOLD 500

extern float bat_v;

extern void read_bat_v(uint8_t bat_v_pin = _BAT_V_PIN);
extern bool  check_5v(uint8_t _5v_pin = _5V_PIN);
