#include <SPI.h>

int LORA_EN_PIN = A3;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // TODO: choose different frequency for different regions
  pinMode(LORA_EN_PIN, OUTPUT);
  digitalWrite(LORA_EN_PIN, HIGH);
  delay(500); // FZ: must add a delay to make sure the power is ready.

  pinMode(PIN_SERIAL_TX, OUTPUT);
  digitalWrite(PIN_SERIAL_TX, LOW);

  SPI.begin();
}

void SPI_tst (void) {
  digitalWrite(PIN_SERIAL_TX, LOW);
  SPI.transfer(0x13);
  SPI.transfer(0x00);
  SPI.endTransaction();
  digitalWrite(PIN_SERIAL_TX, HIGH);
}

// the loop function runs over and over again forever
void loop() {
  SPI_tst();
  delay(1000);                      // wait for a second
}
