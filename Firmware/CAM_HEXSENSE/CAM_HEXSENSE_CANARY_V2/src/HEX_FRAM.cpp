#include <Arduino.h>

#include "HEX_FRAM.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

volatile uint16_t FRAM_w_P            = 0;
volatile uint16_t FRAM_r_P            = 0;
volatile uint16_t FRAM_IMAGE_ADDR_TOP = 0;

Adafruit_FRAM_SPI fram = Adafruit_FRAM_SPI(FRAM_CS);

void fram_setup(void) {
// ===================== FRAM =====================
  fram.begin();
}

bool fram_write(uint16_t addr, uint8_t * data, uint16_t size) {
  fram.writeEnable(true);

  return fram.write(addr, data, size);
}

void fram_read(uint16_t addr, uint8_t * data, uint16_t size) {
  fram.read(addr, data, size);
}

void fram_write_byte(uint16_t addr, uint8_t * data)
{
  fram_write(addr, data, 1);
}

void fram_read_byte(uint16_t addr, uint8_t * data)
{
  fram_read(addr, data, 1);
}

#ifdef __cplusplus
}
#endif

