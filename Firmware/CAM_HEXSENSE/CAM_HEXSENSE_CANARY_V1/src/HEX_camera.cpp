#include "HEX_camera.h"
#include "HEX_system.h"
#include "HEX_FRAM.h"

FZ_ArduCAM_Mega hex_camera;

TCA9534 CAM_CTRL_GPIO;

byte image_buffer[BUFFER_SIZE] = {0};
byte COBS_encoded_image_buffer[CBOS_BUFFER_SIZE] = {0};

HEX_CAMERA_CTRL_PIN HEX_CAMERA_CTRL_PIN_ARRAY[CAMERA_CNT] = {
  {CAM_EN_CH1, CAM_CS_1},
  {CAM_EN_CH2, CAM_CS_2},
  {CAM_EN_CH3, CAM_CS_3},
  {CAM_EN_CH4, CAM_CS_4},
  {CAM_EN_CH5, CAM_CS_5},
  {CAM_EN_CH6, CAM_CS_6}
};

void camera_init() {
  pinMode(CAM_DCDC_EN, OUTPUT);
  digitalWrite(CAM_DCDC_EN, HIGH); // Power off the camera module

  Wire.begin();

  if (CAM_CTRL_GPIO.begin(Wire, _TCA9534_ADDR)) {
    output_debug_info("CAM TCA9534 initialized");
  } else {
    output_debug_info("CAM TCA9534 initialization failed");
  }

  // Power off all cameras
  for (int iii = 0; iii < CAMERA_CNT; iii ++) {
    CAM_CTRL_GPIO.pinMode(HEX_CAMERA_CTRL_PIN_ARRAY[iii].cam_en, GPIO_OUT);
    CAM_CTRL_GPIO.digitalWrite(HEX_CAMERA_CTRL_PIN_ARRAY[iii].cam_en, LOW);
  }

  output_debug_info("Camera initialized");
}

void camera_power_on(int camera_num) {
  // Power on a camera module
  CAM_CTRL_GPIO.digitalWrite(HEX_CAMERA_CTRL_PIN_ARRAY[camera_num].cam_en, HIGH);
  delay(100); // FZ: must add a delay to make sure the power is ready.
  // Wire.begin();
  // CAM_CTRL_GPIO.begin(Wire, _TCA9534_ADDR);

  output_debug_info("Camera powered ON");
}

void camera_power_off(int camera_num) {
  // Power on a camera module
  CAM_CTRL_GPIO.digitalWrite(HEX_CAMERA_CTRL_PIN_ARRAY[camera_num].cam_en, LOW);
  delay(100); // FZ: must add a delay to make sure the power is ready.
  // Wire.begin();
  // CAM_CTRL_GPIO.begin(Wire, _TCA9534_ADDR);

  output_debug_info("Camera powered OFF");
}

void camera_setup(int camera_num) {
  output_debug_info("Camera setup");
  hex_camera.set_CS_pin(HEX_CAMERA_CTRL_PIN_ARRAY[camera_num].cam_cs);

  hex_camera.begin();
  hex_camera.set_format(HEX_CAMERA_FORMAT);
  hex_camera.set_resolution(HEX_CAMERA_RESOLUTION);

  output_debug_info("Camera setup done");
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
  while (hex_camera.getTotalLength() < 40000)
  {
    take_picture();
    delay(200);
  }
}

void get_photo() {
  hex_camera.getpicture();
}

void take_photo_from(int camera_num)
{
  FRAM_w_P = 0;
  FRAM_r_P = 0;

  camera_power_on(camera_num);
  camera_setup(camera_num);
  tst_function();
  camera_power_off(camera_num);
}