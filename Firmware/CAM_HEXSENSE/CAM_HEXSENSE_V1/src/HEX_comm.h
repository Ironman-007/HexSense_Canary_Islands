#pragma once

#include <Arduino.h>

#include <pb_encode.h>
#include <pb_decode.h>

#include <SPI.h>
#include <LoRa.h>

#include "env_comm.pb.h"
#include "HEX_system.h"

#define LORA_EN_PIN  A3
#define LORA_CS_PIN  PIN_SERIAL_TX
#define LORA_IRQ_PIN 4

#define HEXSENSE_ID 10

#define LORA_FREQ_US 915E6
#define LORA_FREQ_EU 868E6

extern void lora_setup(void);
extern void pack_package(void);

extern void lora_sleep(void);
extern void lora_idle(void);

