#pragma once

#include <Arduino.h>
#include <SPI.h>

#define FZ_CAMERA_CS_1 A5
#define FZ_CAMERA_CS_2 A4
#define FZ_CAMERA_CS_3 A3
#define FZ_CAMERA_CS_4 A2
#define FZ_CAMERA_CS_5 A1
#define FZ_CAMERA_CS_6 A0

// FZ: register map, stole from ArduCAM Arduino library
#define CAM_REG_POWER_CONTROL      0X02
#define ARDUCHIP_FIFO              0x04 // FIFO and I2C control
#define CAM_REG_SENSOR_RESET       0X07
#define CAM_REG_FORMAT             0X20
#define CAM_REG_CAPTURE_RESOLUTION 0X21
#define ARDUCHIP_TRIG              0x44 // Trigger source
#define FIFO_SIZE1                 0x45 // Camera write FIFO size[7:0] for burst to read
#define FIFO_SIZE2                 0x46 // Camera write FIFO size[15:8]
#define FIFO_SIZE3                 0x47 // Camera write FIFO size[18:16]

#define FIFO_CLEAR_ID_MASK         0x01 // cameraClearFifoFlag
#define FIFO_START_MASK            0x02
#define FIFO_CLEAR_MASK            0x80
#define CAP_DONE_MASK              0x04

#define CAM_REG_SENSOR_STATE       0x44
#define CAM_REG_SENSOR_STATE_IDLE  (1 << 1)

#define CAM_REG_DEBUG_DEVICE_ADDRESS 0X0A

#define BURST_FIFO_READ              0x3C // Burst FIFO read operation

#define CAM_REG_BRIGHTNESS_CONTROL   0X22

#define SET_EXPOSURE                 0X01
#define CAM_REG_EXPOSURE_GAIN_WHILEBALANCE_CONTROL 0X30

#define CAM_REG_MANUAL_EXPOSURE_BIT_19_16 0X33
#define CAM_REG_MANUAL_EXPOSURE_BIT_15_8  0X34
#define CAM_REG_MANUAL_EXPOSURE_BIT_7_0   0X35

#define CAM_REG_IMAGE_QUALITY             0x2A

#define SET_GAIN                          0X00

#define CAM_SET_CAPTURE_MODE              (0 << 7)
#define CAM_REG_WHILEBALANCE_MODE_CONTROL 0X26

#define SET_WHILEBALANCE                  0X02

/**
 * @enum CAM_IMAGE_MODE
 * @brief Configure camera resolution
 */
typedef enum {
    CAM_IMAGE_MODE_QQVGA  = 0x00,  /**<160x120 */
    CAM_IMAGE_MODE_QVGA   = 0x01,  /**<320x240*/
    CAM_IMAGE_MODE_VGA    = 0x02,  /**<640x480*/
    CAM_IMAGE_MODE_SVGA   = 0x03,  /**<800x600*/
    CAM_IMAGE_MODE_HD     = 0x04,  /**<1280x720*/
    CAM_IMAGE_MODE_SXGAM  = 0x05,  /**<1280x960*/
    CAM_IMAGE_MODE_UXGA   = 0x06,  /**<1600x1200*/
    CAM_IMAGE_MODE_FHD    = 0x07,  /**<1920x1080*/
    CAM_IMAGE_MODE_QXGA   = 0x08,  /**<2048x1536*/
    CAM_IMAGE_MODE_WQXGA2 = 0x09,  /**<2592x1944*/
    CAM_IMAGE_MODE_96X96  = 0x0a,  /**<96x96*/
    CAM_IMAGE_MODE_128X128 = 0x0b, /**<128x128*/
    CAM_IMAGE_MODE_320X320 = 0x0c, /**<320x320*/
    /// @cond
    CAM_IMAGE_MODE_12      = 0x0d, /**<Reserve*/
    CAM_IMAGE_MODE_13      = 0x0e, /**<Reserve*/
    CAM_IMAGE_MODE_14      = 0x0f, /**<Reserve*/
    CAM_IMAGE_MODE_15      = 0x10, /**<Reserve*/
    CAM_IMAGE_MODE_NONE,
    /// @endcond
} CAM_IMAGE_MODE;

/**
 * @enum CAM_IMAGE_PIX_FMT
 * @brief Configure image pixel format
 */
typedef enum {
    CAM_IMAGE_PIX_FMT_RGB565 = 0x02, /**< RGB565 format */
    CAM_IMAGE_PIX_FMT_JPG    = 0x01, /**< JPEG format */
    CAM_IMAGE_PIX_FMT_YUV    = 0x03, /**< YUV format */
    CAM_IMAGE_PIX_FMT_NONE,          /**< No defined format */
} CAM_IMAGE_PIX_FMT;

/**
 * @enum CAM_BRIGHTNESS_LEVEL
 * @brief Configure camera brightness level
 */
typedef enum {
    CAM_BRIGHTNESS_LEVEL_MINUS_4 = 8, /**<Level -4 */
    CAM_BRIGHTNESS_LEVEL_MINUS_3 = 6, /**<Level -3 */
    CAM_BRIGHTNESS_LEVEL_MINUS_2 = 4, /**<Level -2 */
    CAM_BRIGHTNESS_LEVEL_MINUS_1 = 2, /**<Level -1 */
    CAM_BRIGHTNESS_LEVEL_DEFAULT = 0, /**<Level Default*/
    CAM_BRIGHTNESS_LEVEL_1       = 1, /**<Level +1 */
    CAM_BRIGHTNESS_LEVEL_2       = 3, /**<Level +2 */
    CAM_BRIGHTNESS_LEVEL_3       = 5, /**<Level +3 */
    CAM_BRIGHTNESS_LEVEL_4       = 7, /**<Level +4 */
} CAM_BRIGHTNESS_LEVEL;

/**
 * @enum CAM_WHITE_BALANCE
 * @brief Configure white balance mode
 */
typedef enum {
    CAM_WHITE_BALANCE_MODE_DEFAULT = 0, /**< Auto */
    CAM_WHITE_BALANCE_MODE_SUNNY,       /**< Sunny */
    CAM_WHITE_BALANCE_MODE_OFFICE,      /**< Office */
    CAM_WHITE_BALANCE_MODE_CLOUDY,      /**< Cloudy*/
    CAM_WHITE_BALANCE_MODE_HOME,        /**< Home */
} CAM_WHITE_BALANCE;

typedef enum {
    HIGH_QUALITY    = 0,
    DEFAULT_QUALITY = 1,
    LOW_QUALITY     = 2,
} IMAGE_QUALITY;

#define CAM_SENSOR_RESET_ENABLE (1 << 6)

#define CAM_REG_MANUAL_GAIN_BIT_9_8 0X31
#define CAM_REG_MANUAL_GAIN_BIT_7_0 0X32

#define CAMERA_CS A5

class FZ_ArduCAM_Mega
{
private:
  int      _cs;
  uint32_t _total_length;
  uint32_t _buffer_size;

  uint8_t _deviceAddress = 0.78;

public:
  bool burstFirstFlag = 0;

  FZ_ArduCAM_Mega(int pin_cs = CAMERA_CS);
  ~FZ_ArduCAM_Mega();

  void begin(void);

  void set_CS_pin(int pin_cs);

  void write_register(uint8_t addr, uint8_t data);
  uint8_t read_register(uint8_t addr);
  uint8_t getBit(uint8_t addr, uint8_t bit);

  void power_on(void);
  void power_off(void);

  void reset_camera(void);
  void waitI2cIdle(void);

  void set_format(uint8_t fmt);
  void set_resolution(uint8_t resolution);
  void set_brightness(CAM_BRIGHTNESS_LEVEL brightness);
  void set_exporsure(uint32_t exposure_time);
  void setAutoExposure(uint8_t val);
  void set_jpeg_quality(IMAGE_QUALITY quality);
  void SetAutoISOSensitive(uint8_t val);
  void SetAutoWhiteBalanceMode(uint8_t val);
  void SetWhiteBalanceMode(CAM_WHITE_BALANCE WB_mode);
  void SetISOValue(int iso_val);

  uint32_t ReadFifoLength(void);

  void SetCapture(void);

  uint32_t getTotalLength(void);

  void getpicture(void);

  uint32_t read_buffer(uint8_t *buff, uint32_t len);
  void setFifoBurst(void);
};

extern FZ_ArduCAM_Mega FZ_camera_1;
extern FZ_ArduCAM_Mega FZ_camera_2;
extern FZ_ArduCAM_Mega FZ_camera_3;
extern FZ_ArduCAM_Mega FZ_camera_4;
extern FZ_ArduCAM_Mega FZ_camera_5;
extern FZ_ArduCAM_Mega FZ_camera_6;
