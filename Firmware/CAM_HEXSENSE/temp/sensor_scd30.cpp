#include <Arduino.h>

#include "system.h"
#include "sensor_scd30.h"

Adafruit_SCD30  scd30;

volatile float scd_temperature = 0.0;
volatile float scd_humidity    = 0.0;
volatile float scd_co2         = 0.0;

void sensor_scd30_setup(void) {
  pinMode(SCCD30_EN_PIN, OUTPUT);
  digitalWrite(SCCD30_EN_PIN, HIGH);
  delay(500); // FZ: must add a delay to make sure the power is ready.

  scd30.begin();
  scd30.setMeasurementInterval(30);

  if (SERIAL_DEBUG) {
    Serial.println("sensor_scd30_setup");
  }
}

void scd30_poweroff(void) {
  digitalWrite(SCCD30_EN_PIN, LOW);
}

bool read_scd30(void) {
  if (scd30.dataReady()) {
    scd30.read();

    scd_temperature = scd30.temperature;
    scd_humidity    = scd30.relative_humidity;
    scd_co2         = scd30.CO2;

    if (SERIAL_DEBUG) {
      Serial.print("Temperature: ");
      Serial.print(scd_temperature);
      Serial.print("C, Humidity: ");
      Serial.print(scd_humidity);
      Serial.print("%, CO2: ");
      Serial.print(scd_co2);
      Serial.println("ppm");
    }

    return true;
  }
  else {
    if (SERIAL_DEBUG) {
      Serial.println("No data");
    }
    return false;
  }
}
