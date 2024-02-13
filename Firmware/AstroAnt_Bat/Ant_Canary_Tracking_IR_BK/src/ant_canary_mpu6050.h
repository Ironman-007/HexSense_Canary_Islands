#pragma once

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

extern Adafruit_MPU6050 mpu;

extern float gyro_z_bias;
#define CALI_CNT 10

extern float _AGZ;

extern float gyroZrate;

extern float gyro_z_bias;

extern int imu_cali_num;

extern sensors_event_t mpu_a, mpu_g, mpu_temp;

extern void IMU_setup(void);

extern void IMU_cali(void);

#ifdef __cplusplus
}
#endif