#pragma once

#include <Arduino.h>
#include <bluefruit.h>
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define MAX_PACKAGE_LEN 200

// ====================== BLE parameters ======================
// BLE Service
extern BLEDis  bledis;   // device information
extern BLEUart bleuart;  // uart over ble

extern volatile int CS_CONNECTED; // uart over ble

// ===================== BLE functions =====================
extern void startAdv(void);

extern void ble_setup(void);
extern void rx_callback(uint16_t conn_handle);

extern void connect_callback(uint16_t conn_handle);
extern void disconnect_callback(uint16_t conn_handle, uint8_t reason);

extern void ant_comm_update(void);

#ifdef __cplusplus
}
#endif