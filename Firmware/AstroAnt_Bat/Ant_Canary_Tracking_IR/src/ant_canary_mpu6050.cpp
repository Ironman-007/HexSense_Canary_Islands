// comm_parameters.c - variables and functions related to communication, including code/decode packets.
#include <Arduino.h>
#include <stdint.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include "ant_canary_mpu6050.h"
#include "system.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

float _AGZ = 0.0;

float gyroZrate = 0.0;

float gyro_z_buffer[CALI_CNT] = {0.0};

float gyro_z_sum = 0.0;
float gyro_z_bias = 0.0;

int imu_cali_num = CALI_CNT;

Adafruit_MPU6050 mpu;

sensors_event_t mpu_a, mpu_g, mpu_temp;

int imu_i = CALI_CNT-1;

void IMU_setup() {
  // ============== MPU6050 ==============
  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);
  delay(10);

  output_debug_info("MPU6050 setup done");
}

// IMU calibration - Gyroscope calibration
void IMU_cali(void) {
  imu_cali_num = CALI_CNT;

  gyro_z_sum = 0;

  while (imu_cali_num > 0) {
    if (timer4Interrupt_100ms) {
      timer4Interrupt_100ms = false;

      for (imu_i = CALI_CNT-1; imu_i > 0; imu_i--) {
        gyro_z_buffer[imu_i] = gyro_z_buffer[imu_i - 1];
      }

      mpu.getEvent(&mpu_a, &mpu_g, &mpu_temp);

      gyro_z_buffer[0] = mpu_g.gyro.z;

      imu_cali_num --;
    }
  }

  for (imu_i = 0; imu_i < CALI_CNT; imu_i ++) gyro_z_sum += gyro_z_buffer[imu_i];

  gyro_z_bias = gyro_z_sum/CALI_CNT;

  output_debug_info_float("Gyroscope bias: ", gyro_z_bias);
}

#ifdef __cplusplus
}
#endif
