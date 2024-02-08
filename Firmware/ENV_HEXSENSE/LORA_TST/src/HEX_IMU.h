#pragma once

#include <Arduino.h>
#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20948.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

extern Adafruit_ICM20948 icm;

extern float acc_x;
extern float acc_y;
extern float acc_z;
extern float body_orientation;

extern void IMU_power_ON(void);
extern void IMU_power_OFF(void);

extern void imu_setup(void);
extern void read_imu(void);

extern void calculate_orientation(void);