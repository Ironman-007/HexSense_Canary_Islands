#include <Arduino.h>
#include <Wire.h>

#include "FZ_ArduCAM_Mega.h"
#include "SparkFun_TCA9534.h"
#include "HEX_comm.h"

extern FZ_ArduCAM_Mega mycamera;

#define CAM_DCDC_EN           A3

#define HEX_CAMERA_RESOLUTION CAM_IMAGE_MODE_SXGAM
#define HEX_CAMERA_FORMAT     CAM_IMAGE_PIX_FMT_JPG

#define BUFFER_SIZE      100
#define CBOS_BUFFER_SIZE 150

extern byte image_buffer[BUFFER_SIZE];
extern byte COBS_encoded_image_buffer[CBOS_BUFFER_SIZE];

extern TCA9534        CAM_CTRL_GPIO;
#define _TCA9534_ADDR 0x20
#define CAMERA_CNT    6

#define CAM_EN_CH1    6
#define CAM_EN_CH2    5
#define CAM_EN_CH3    4
#define CAM_EN_CH4    3
#define CAM_EN_CH5    2
#define CAM_EN_CH6    1

#define CAM_CS_1      A2
#define CAM_CS_2      A5
#define CAM_CS_3      12
#define CAM_CS_4      3
#define CAM_CS_5      1
#define CAM_CS_6      2

#define CAM_1         0
#define CAM_2         1
#define CAM_3         2
#define CAM_4         3
#define CAM_5         4
#define CAM_6         5

struct HEX_CAMERA_CTRL_PIN
{
    int cam_en;
    int cam_cs;
};

extern HEX_CAMERA_CTRL_PIN HEX_CAMERA_CTRL_PIN_ARRAY[CAMERA_CNT];

extern void camera_init(void);
extern void camera_power_on(int camera_num);
extern void camera_power_off(int camera_num);
extern void camera_setup(int camera_num);
extern void take_picture(void);
extern void capture_photo(void);
extern void get_photo(void);

extern void take_photo_from(int camera_num);
