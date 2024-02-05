#pragma once

#include <Arduino.h>
#include <Adafruit_SCD30.h>

#define SCCD30_EN_PIN A3

extern Adafruit_SCD30  scd30;

extern volatile float scd_temperature;
extern volatile float scd_humidity;
extern volatile float scd_co2;

extern void sensor_scd30_setup(void);
extern void scd30_poweroff(void);

extern bool read_scd30(void);