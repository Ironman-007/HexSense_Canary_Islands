#pragma once

#include <Arduino.h>
#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20948.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

extern Adafruit_ICM20948 icm;

extern void imu_setup(void);
extern void read_imu(void);