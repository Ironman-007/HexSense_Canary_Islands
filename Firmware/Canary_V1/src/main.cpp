#include <Arduino.h>

void setup() {
  // setup led pin as output
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // turn on led
  digitalWrite(LED_BUILTIN, HIGH);
  // wait 1 second
  delay(1000);
  // turn off led
  digitalWrite(LED_BUILTIN, LOW);
  // wait 1 second
  delay(100);
}

