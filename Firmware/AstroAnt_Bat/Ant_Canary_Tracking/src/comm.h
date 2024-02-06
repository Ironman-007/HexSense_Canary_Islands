#pragma once

#include <Arduino.h>
#include <bluefruit.h>
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>

#include <pb_encode.h>
#include <pb_decode.h>

#include "ant_canary.pb.h"

extern volatile bool bleConnected;

// BLE Service
extern BLEDfu  bledfu;  // OTA DFU service
extern BLEDis  bledis;  // device information
extern BLEUart bleuart; // uart over ble
extern BLEBas  blebas;  // battery

extern void startAdv(void);

// callback invoked when central connects
extern void connect_callback(uint16_t conn_handle);

/**
 * Callback invoked when a connection is dropped
 * @param conn_handle connection where this event happens
 * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
 */
extern void disconnect_callback(uint16_t conn_handle, uint8_t reason);

extern void ble_setup(void);

enum Ant_cmd_type {
  Ant_cmd_type_none = 0,
  Ant_cmd_type_motor,
  Ant_cmd_type_led,
  Ant_cmd_type_buzzer,
  Ant_cmd_type_battery,
  Ant_cmd_type_ota,
  Ant_cmd_type_reset,
  Ant_cmd_type_debug,
  Ant_cmd_type_unknow
};

extern uint8_t decodecmd(const uint8_t* data, size_t size);