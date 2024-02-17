#include "FZ_ArduCAM_Mega.h"
#include "HEX_system.h"
#include "HEX_camera.h"
#include "HEX_FRAM.h"
#include "HEX_comm.h"

FZ_ArduCAM_Mega::FZ_ArduCAM_Mega(int pin_cs) {
  _cs = pin_cs;

  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH);
}

FZ_ArduCAM_Mega::~FZ_ArduCAM_Mega() {
  ;
}

void FZ_ArduCAM_Mega::begin(void) {
  SPI.begin();
  reset_camera();
}

void FZ_ArduCAM_Mega::set_CS_pin(int pin_cs) {
  this -> _cs = pin_cs;

  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH);
}

void FZ_ArduCAM_Mega::write_register(uint8_t addr, uint8_t data) {
  digitalWrite(_cs, LOW);
  SPI.transfer(addr | 0x80);
  SPI.transfer(data);
  digitalWrite(_cs, HIGH);
}

void FZ_ArduCAM_Mega::set_format(uint8_t fmt) {
  write_register(CAM_REG_FORMAT, fmt);
  waitI2cIdle();
  output_debug_info("Setting format done.");
}

void FZ_ArduCAM_Mega::SetAutoWhiteBalanceMode(uint8_t val) {
  unsigned char symbol = 0;
  if (val == 1) {
      symbol |= 0x80;
  }
  symbol |= SET_WHILEBALANCE;
  write_register(CAM_REG_EXPOSURE_GAIN_WHILEBALANCE_CONTROL, symbol);    // while balance control
  waitI2cIdle(); // Wait I2c Idle
}

void FZ_ArduCAM_Mega::SetWhiteBalanceMode(CAM_WHITE_BALANCE WB_mode) {
  write_register(CAM_REG_WHILEBALANCE_MODE_CONTROL, WB_mode); // set Light Mode
  waitI2cIdle();                                       // Wait I2c Idle
}

void FZ_ArduCAM_Mega::set_resolution(uint8_t resolution) {
  write_register(CAM_REG_CAPTURE_RESOLUTION, CAM_SET_CAPTURE_MODE | resolution);
  waitI2cIdle();
  output_debug_info("Setting resolution done.");
}

void FZ_ArduCAM_Mega::set_brightness(CAM_BRIGHTNESS_LEVEL brightness) {
  write_register(CAM_REG_BRIGHTNESS_CONTROL, brightness);
  waitI2cIdle();
}

void FZ_ArduCAM_Mega::setAutoExposure(uint8_t val)
{
  unsigned char symbol = 0;
  if (val == 1) {
      symbol |= 0x80;
  }
  symbol |= SET_EXPOSURE;
  write_register(CAM_REG_EXPOSURE_GAIN_WHILEBALANCE_CONTROL, symbol);    // auto EXPOSURE control
  waitI2cIdle(); // Wait I2c Idle
}

void FZ_ArduCAM_Mega::set_exporsure(uint32_t exposure_time)
{
  // set exposure output [19:16]
  write_register(CAM_REG_MANUAL_EXPOSURE_BIT_19_16, (uint8_t)((exposure_time >> 16) & 0xff));
  waitI2cIdle();
  // set exposure output [15:8]
  write_register(CAM_REG_MANUAL_EXPOSURE_BIT_15_8, (uint8_t)((exposure_time >> 8) & 0xff));
  waitI2cIdle();
  // set exposure output [7:0]
  write_register(CAM_REG_MANUAL_EXPOSURE_BIT_7_0, (uint8_t)(exposure_time & 0xff));
  waitI2cIdle();

  output_debug_info("Setting exposure done.");

  // Serial.println(read_register(CAM_REG_MANUAL_EXPOSURE_BIT_19_16));
  // Serial.println(read_register(CAM_REG_MANUAL_EXPOSURE_BIT_15_8));
  // Serial.println(read_register(CAM_REG_MANUAL_EXPOSURE_BIT_7_0));
}

void FZ_ArduCAM_Mega::set_jpeg_quality(IMAGE_QUALITY quality)
{
  write_register(CAM_REG_IMAGE_QUALITY, quality);
  waitI2cIdle(); // Wait I2c Idle
}

void FZ_ArduCAM_Mega::SetAutoISOSensitive(uint8_t val)
{
  unsigned char symbol = 0;
    if (val == 1) {
        symbol |= 0x80;
    }
    symbol |= SET_GAIN;
    write_register(CAM_REG_EXPOSURE_GAIN_WHILEBALANCE_CONTROL, symbol);    // auto gain control
    waitI2cIdle(); // Wait I2c Idle
}

uint8_t ov3640GainValue[] = {0x00, 0x10, 0x18, 0x30, 0x34, 0x38, 0x3b, 0x3f, 0x72, 0x74, 0x76,
                             0x78, 0x7a, 0x7c, 0x7e, 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6,
                             0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff};

void FZ_ArduCAM_Mega::SetISOValue(int iso_val)
{
    // iso_val = ov3640GainValue[iso_val - 1];

    write_register(CAM_REG_MANUAL_GAIN_BIT_9_8, iso_val >> 8); // set AGC VALUE
    waitI2cIdle();
    write_register(CAM_REG_MANUAL_GAIN_BIT_7_0, iso_val & 0xff);
    waitI2cIdle();
}

/*
uint8_t cameraReadReg(ArducamCamera* camera, uint8_t addr)
{
    uint8_t data;
    data = cameraBusRead(camera, addr & 0x7F);
    return data;
}

uint8_t cameraBusRead(ArducamCamera* camera, int address)
{
    uint8_t value;
    arducamSpiCsPinLow(camera->csPin);
    arducamSpiTransfer(address);
    value = arducamSpiTransfer(0x00);
    value = arducamSpiTransfer(0x00);
    arducamSpiCsPinHigh(camera->csPin);
    return value;
}
*/

uint8_t FZ_ArduCAM_Mega::read_register(uint8_t addr)
{
  uint8_t data;

  digitalWrite(_cs, LOW);
  SPI.transfer(addr & 0x7F);
  data = SPI.transfer(0x00);
  data = SPI.transfer(0x00); // ArduCAM Mega needs to send 2 bytes to read the register, first one is a dummy byte
  digitalWrite(_cs, HIGH);

  return data;
}

uint8_t FZ_ArduCAM_Mega::getBit(uint8_t addr, uint8_t bit)
{
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

void FZ_ArduCAM_Mega::SetCapture(void)
{
  write_register(ARDUCHIP_FIFO, FIFO_CLEAR_ID_MASK); // clearFifoFlag
  write_register(ARDUCHIP_FIFO, FIFO_START_MASK);    // start capture

  while (getBit(ARDUCHIP_TRIG, CAP_DONE_MASK) == 0) {
    delay(10);
  }

  uint32_t length = ReadFifoLength();
  this -> _total_length = length;
  this -> _buffer_size  = length;

  output_debug_info_int32("Capture done, total length = ", length);
}

uint32_t FZ_ArduCAM_Mega::getTotalLength(void) {
  return this -> _buffer_size;
}

void FZ_ArduCAM_Mega::reset_camera(void) {
  output_debug_info("Camera reset.");

  write_register(CAM_REG_SENSOR_RESET, CAM_SENSOR_RESET_ENABLE);
  waitI2cIdle();

  output_debug_info("Camera reset done.");

  write_register(CAM_REG_DEBUG_DEVICE_ADDRESS, _deviceAddress);
  waitI2cIdle();

  output_debug_info("Camera _deviceAddress set done.");
}

void FZ_ArduCAM_Mega::waitI2cIdle(void)
{
  while ((read_register(CAM_REG_SENSOR_STATE) & 0X03) != CAM_REG_SENSOR_STATE_IDLE)
  {
    delayMicroseconds(10);
  }
}

void FZ_ArduCAM_Mega::getpicture(void)
{
  FRAM_w_P = 0;

  while (getTotalLength())
  {
    read_buffer(NULL, BUFFER_SIZE);

    // print all data in HEX in the image_buffer
    if (SERIAL_DEBUG)
    {
      for (int iii = 0; iii < BUFFER_SIZE; iii++)
      {
        if (image_buffer[iii] < 0x10)
        {
          Serial.print("0");
        }
        Serial.print(image_buffer[iii], HEX);
      }
    }

    fram_write(FRAM_w_P, image_buffer, BUFFER_SIZE);

    output_debug_info("FRAM write done.");
    uint8_t temp_read_byte = 0;
    fram_read_byte(FRAM_w_P, &temp_read_byte);

    if (temp_read_byte < 0x10)
    {
      Serial.print("0");
    }
    Serial.println(temp_read_byte, HEX);

    FRAM_w_P += BUFFER_SIZE;

    delay(20);
  }

  FRAM_IMAGE_ADDR_TOP = FRAM_w_P;
  output_debug_info_int32("FRAM_IMAGE_ADDR_TOP: ", FRAM_IMAGE_ADDR_TOP);
}

void FZ_ArduCAM_Mega::setFifoBurst(void)
{
  SPI.transfer(BURST_FIFO_READ);
}

/*
uint32_t FZ_ArduCAM_Mega::read_buffer(uint8_t *buff, uint32_t len)
{
  uint32_t length = len;

  if (getTotalLength() == 0 || (len == 0))
  {
    return 0;
  }

  if (getTotalLength() < len)
  {
    len = getTotalLength();
  }

  digitalWrite(_cs, LOW);
  setFifoBurst();

  if (this->burstFirstFlag == 0)
  {
    this->burstFirstFlag = 1;
    SPI.transfer(0x00);
  }

  for (uint32_t count = 0; count < length; count++)
  {
    if (count < len)
    {
      byte temp_byte = SPI.transfer(0x00);
      image_buffer[count] = temp_byte;

      if (SERIAL_DEBUG)
      {
        if (temp_byte < 0x10)
        {
          Serial.print("0");
        }
        Serial.print(temp_byte, HEX);
      }
    }
    else if (count == len)
    {
      digitalWrite(_cs, HIGH);
      image_buffer[count] = 0x00;
    }
    else
    {
      image_buffer[count] = 0x00;
    }
  }

  this->_buffer_size -= len;

  return len;
}
*/

uint32_t FZ_ArduCAM_Mega::read_buffer(uint8_t *buff, uint32_t len)
{
  uint32_t length = len;
  uint32_t count = 0;

  if (getTotalLength() == 0 || (len == 0))
  {
    return 0;
  }

  if (getTotalLength() < len)
  {
    len = getTotalLength();
  }

  digitalWrite(_cs, LOW);
  setFifoBurst();

  if (this->burstFirstFlag == 0)
  {
    this->burstFirstFlag = 1;
    SPI.transfer(0x00);
  }

  for (count = 0; count < length; count++)
  {
    if (count < len)
    {
      byte temp_byte = SPI.transfer(0x00);
      image_buffer[count] = temp_byte;

      // if (temp_byte < 0x10) {
      //   Serial.print("0");
      // }
      // Serial.print(temp_byte, HEX);

    }
    else
    {
      image_buffer[count] = 0x00;

      // byte temp_byte = 0x00;
      // if (temp_byte < 0x10) {
      //   Serial.print("0");
      // }
      // Serial.print(temp_byte, HEX);

    }
  }

  digitalWrite(_cs, HIGH);

  this->_buffer_size -= len;

  return len;
}

void FZ_ArduCAM_Mega::power_on(void)
{
  ;
}

void FZ_ArduCAM_Mega::power_off(void) {
  ;
}

FZ_ArduCAM_Mega FZ_camera_1(FZ_CAMERA_CS_1);
FZ_ArduCAM_Mega FZ_camera_2(FZ_CAMERA_CS_2);
FZ_ArduCAM_Mega FZ_camera_3(FZ_CAMERA_CS_3);
FZ_ArduCAM_Mega FZ_camera_4(FZ_CAMERA_CS_4);
FZ_ArduCAM_Mega FZ_camera_5(FZ_CAMERA_CS_5);
FZ_ArduCAM_Mega FZ_camera_6(FZ_CAMERA_CS_6);
