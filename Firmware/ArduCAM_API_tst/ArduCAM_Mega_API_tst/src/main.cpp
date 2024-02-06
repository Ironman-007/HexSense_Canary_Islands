#include <Arduino.h>

#include "FZ_ArduCAM_Mega.h"

FZ_ArduCAM_Mega mycamera;

CAM_IMAGE_MODE resolution = CAM_IMAGE_MODE_HD;
CAM_IMAGE_PIX_FMT format  = CAM_IMAGE_PIX_FMT_JPG;

void take_picture() {
  mycamera.set_exporsure(10000);
  // mycamera.setAutoExposure(1);
  mycamera.SetAutoISOSensitive(1);
  mycamera.SetAutoWhiteBalanceMode(1);
  // mycamera.setAutoExposure(1);
  mycamera.SetCapture();

  delay(1000);
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  mycamera.begin();

  mycamera.set_format(format);
  mycamera.set_resolution(resolution);

  take_picture();
  take_picture();
  take_picture();
  take_picture();
  take_picture();

  mycamera.getpicture();

  Serial.println("");
  Serial.println("Done.");
}

void loop() {
  delay(10000);
}
