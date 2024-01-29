#include <Arduino.h>

#include "pb_encode.h"
#include "pb_decode.h"

uint8_t      buffer[20];
pb_ostream_t ostream;
pb_istream_t istream;
size_t       written;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  // use nanopb to encode data stream

}
