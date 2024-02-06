#include <Arduino.h>

#include "FZ_ArduCAM_Mega.h"

FZ_ArduCAM_Mega mycamera;

CAM_IMAGE_MODE resolution = CAM_IMAGE_MODE_HD;
CAM_IMAGE_PIX_FMT format  = CAM_IMAGE_PIX_FMT_JPG;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  mycamera.begin();

  mycamera.set_format(format);
  mycamera.set_resolution(resolution);
  mycamera.set_brightness(CAM_BRIGHTNESS_LEVEL_4);
  mycamera.set_exporsure(1000);
  // mycamera.setAutoExposure(1);
  mycamera.SetCapture();

  mycamera.getpicture();

  Serial.println("");
  Serial.println("Done.");
}

void loop() {
  delay(100); 
}
