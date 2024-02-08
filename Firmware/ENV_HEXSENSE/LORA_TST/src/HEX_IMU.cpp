#include <Arduino.h>

#include "HEX_IMU.h"
#include "HEX_system.h"

Adafruit_ICM20948 icm;
uint16_t measurement_delay_us = 65535; // Delay between measurements for testing

float acc_x = 0.0;
float acc_y = 0.0;
float acc_z = 0.0;
float body_orientation = 0.0;

void IMU_power_ON(void) {
  pinMode(PIN_LED_TXL, OUTPUT);
  digitalWrite(PIN_LED_TXL, HIGH);
  delay(100);
}

void IMU_power_OFF(void) {
  digitalWrite(PIN_LED_TXL, LOW);
}

void imu_setup(void) {
  IMU_power_ON();

  // Try to initialize!
  if (!icm.begin_I2C()) {

    Serial.println("Failed to find ICM20948 chip");
    while (1) {
      delay(10);
    }
  }
}

void read_imu(void) {
  //  /* Get a new normalized sensor event */
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t mag;
  sensors_event_t temp;

  icm.getEvent(&accel, &gyro, &temp, &mag);

  acc_x = accel.acceleration.x;
  acc_y = accel.acceleration.y;
  acc_z = accel.acceleration.z;
}

void calculate_orientation(void) {
  //  /* Get a new normalized sensor event */
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t mag;
  sensors_event_t temp;

  icm.getEvent(&accel, &gyro, &temp, &mag);

  acc_x = accel.acceleration.x;
  acc_y = accel.acceleration.y;
  acc_z = accel.acceleration.z;

  // if (SERIAL_DEBUG) {
  //   /* Display the results (acceleration is measured in m/s^2) */
  //   Serial.print("\t\tAccel X: ");
  //   Serial.print(accel.acceleration.x);
  //   Serial.print("\tY: ");
  //   Serial.print(accel.acceleration.y);
  //   Serial.print("\tZ: ");
  //   Serial.print(accel.acceleration.z);
  //   Serial.println(" m/s^2");
  // }

  body_orientation = atan2(accel.acceleration.y, accel.acceleration.x) * 180 / PI;
  if (body_orientation < 0) body_orientation += 360;
}