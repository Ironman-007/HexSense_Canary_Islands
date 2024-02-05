#include <Arduino.h>

#include "FZ_ArduCAM_Mega.h"

FZ_ArduCAM_Mega mycamera;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  mycamera.set_format(0x01);
  mycamera.set_resolution(0x04);
  mycamera.SetCapture();

  Serial.print("Total length = ");
  Serial.println(mycamera.getTotalLength());
}

void loop() {
  delay(100); 
}
