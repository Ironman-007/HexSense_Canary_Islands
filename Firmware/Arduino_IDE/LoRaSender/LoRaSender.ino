#include <SPI.h>
#include <LoRa.h>

int counter = 0;
int LORA_EN_PIN = A3;

void setup() {
  // TODO: choose different frequency for different regions
  pinMode(LORA_EN_PIN, OUTPUT);
  digitalWrite(LORA_EN_PIN, HIGH);
  delay(500); // FZ: must add a delay to make sure the power is ready.

  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Sender");
  LoRa.setPins(PIN_SERIAL_TX, 2, 3);

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    // while (1);
  }
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(5000);
}
