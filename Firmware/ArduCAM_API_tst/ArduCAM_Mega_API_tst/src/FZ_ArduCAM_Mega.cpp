#include "FZ_ArduCAM_Mega.h"

FZ_ArduCAM_Mega::FZ_ArduCAM_Mega(int pin_cs) {
  _cs = pin_cs;

  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH);
}

FZ_ArduCAM_Mega::~FZ_ArduCAM_Mega() {
  ;
}

void FZ_ArduCAM_Mega::set_CS_pin(int pin_cs) {
  this -> _cs = pin_cs;
}

void FZ_ArduCAM_Mega::write_register(uint8_t addr, uint8_t data) {
  digitalWrite(_cs, LOW);
  SPI.transfer(addr | 0x80);
  SPI.transfer(data);
  digitalWrite(_cs, HIGH);
}

void FZ_ArduCAM_Mega::set_format(uint8_t fmt) {
  write_register(CAM_REG_FORMAT, fmt);
}

void FZ_ArduCAM_Mega::set_resolution(uint8_t resolution) {
  write_register(CAM_REG_CAPTURE_RESOLUTION, resolution);
}

uint8_t FZ_ArduCAM_Mega::read_register(uint8_t addr) {
  uint8_t data;

  digitalWrite(_cs, LOW);
  SPI.transfer(addr & 0x7F);
  data = SPI.transfer(0x00);
  digitalWrite(_cs, HIGH);

  return data;
}

uint8_t FZ_ArduCAM_Mega::getBit(uint8_t addr, uint8_t bit) {
  uint8_t temp;
  temp = read_register(addr);
  temp = temp & bit;
  return temp;
}

uint32_t FZ_ArduCAM_Mega::ReadFifoLength(void)
{
    uint32_t len1, len2, len3, length = 0;
    len1   = read_register(FIFO_SIZE1);
    len2   = read_register(FIFO_SIZE2);
    len3   = read_register(FIFO_SIZE3);
    length = ((len3 << 16) | (len2 << 8) | len1) & 0xffffff;
    return length;
}

void FZ_ArduCAM_Mega::SetCapture(void) {
  // flushFifo(camera);
  write_register(ARDUCHIP_FIFO, FIFO_CLEAR_ID_MASK); // clearFifoFlag
  write_register(ARDUCHIP_FIFO, FIFO_START_MASK); // clearFifoFlag

  while (getBit(ARDUCHIP_TRIG, CAP_DONE_MASK) == 0) {
    delay(10);
  }

  this -> _total_length = ReadFifoLength();
}

uint32_t FZ_ArduCAM_Mega::getTotalLength(void) {
  return this -> _total_length;
}