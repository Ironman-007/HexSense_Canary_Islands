#include "Adafruit_TinyUSB.h"

#define BLE_LED_PIN 16
#define MSG_LED_PIN 10

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  while(!Serial) {delay(10);}
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(BLE_LED_PIN, OUTPUT);
  pinMode(MSG_LED_PIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(BLE_LED_PIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(MSG_LED_PIN, LOW);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(BLE_LED_PIN, LOW);   // turn the LED off by making the voltage LOW
  digitalWrite(MSG_LED_PIN, HIGH);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
}
