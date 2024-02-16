#pragma once

#include <Arduino.h>

#include "Adafruit_FRAM_SPI.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

extern volatile uint16_t FRAM_w_P;
extern volatile uint16_t FRAM_r_P;

#define FRAM_CS           6
extern  Adafruit_FRAM_SPI fram;

extern void fram_setup(void);
extern void fram_write(uint16_t addr, uint8_t * data, uint16_t size);
extern void fram_read(uint16_t addr, uint8_t * data, uint16_t size);

extern void fram_write_byte(uint16_t addr, uint8_t * data);
extern void fram_read_byte(uint16_t addr, uint8_t * data);

#ifdef __cplusplus
}
#endif

