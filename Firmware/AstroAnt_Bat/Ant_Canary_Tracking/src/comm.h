#pragma once

#include <Arduino.h>
#include <bluefruit.h>
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>

#include <pb_encode.h>
#include <pb_decode.h>

#include "ant_canary.pb.h"

extern volatile bool bleConnected;

extern uint8_t cmd_buffer[100];

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

extern _CMD_RECV decodecmd(const uint8_t* data, size_t size);

extern _CMD_RECV get_recv_cmd(size_t size);

extern void handle_cmd(_CMD_RECV cmd_recv);