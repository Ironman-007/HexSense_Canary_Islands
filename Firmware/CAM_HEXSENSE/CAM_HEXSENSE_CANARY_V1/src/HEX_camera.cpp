#include "HEX_camera.h"

FZ_ArduCAM_Mega hex_camera;

byte image_buffer[100] = {0};

void camera_setup() {
  hex_camera.begin();
  hex_camera.set_format(HEX_CAMERA_FORMAT);
  hex_camera.set_resolution(HEX_CAMERA_RESOLUTION);
}

void take_picture() {
  hex_camera.setAutoExposure(1);
  hex_camera.SetAutoISOSensitive(1);
  hex_camera.set_brightness(CAM_BRIGHTNESS_LEVEL_1);
  hex_camera.SetWhiteBalanceMode(CAM_WHITE_BALANCE_MODE_OFFICE);
  hex_camera.SetCapture();

  delay(10);
}

void capture_photo() {
  take_picture();
  take_picture();
  take_picture();
  take_picture();
  take_picture();
}

void get_photo() {
  hex_camera.getpicture();
}

