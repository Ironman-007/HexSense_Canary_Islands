#include <Arduino.h>

#include "FZ_ArduCAM_Mega.h"

extern FZ_ArduCAM_Mega mycamera;

#define HEX_CAMERA_RESOLUTION CAM_IMAGE_MODE_SXGAM
#define HEX_CAMERA_FORMAT     CAM_IMAGE_PIX_FMT_JPG

#define BUFFER_SIZE     100

extern byte image_buffer[BUFFER_SIZE];

extern void camera_setup(void);
extern void take_picture(void);
extern void capture_photo(void);

extern void get_photo(void);
