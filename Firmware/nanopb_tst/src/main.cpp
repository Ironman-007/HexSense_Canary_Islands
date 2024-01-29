#include <Arduino.h>

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
