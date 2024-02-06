#include <Arduino.h>

#include "ant_comm.h"
#include "system.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

BLEDis  bledis;
BLEUart bleuart;

volatile int CS_CONNECTED = 0;

// ===================== BLE functions =====================
void ble_setup() {
  Bluefruit.autoConnLed(true);
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
  Bluefruit.begin();
  Bluefruit.setTxPower(4);
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

  Bluefruit.setName("AstroAnt - Lazarote");

  // bledfu.begin();
  bledis.begin();
  bleuart.begin();

  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include bleuart 128-bit uuid
  Bluefruit.Advertising.addService(bleuart);

  // Secondary Scan Response packet (optional)
  Bluefruit.ScanResponse.addName();

  // Start Advertising
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244); // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);   // number of seconds in fast mode

  output_debug_info("BLE setup done");
}

void startAdv(void) {
  Bluefruit.Advertising.start(0); // 0 = Don't stop advertising after n seconds
  output_debug_info("startAdv done");
}

// callback invoked when central connects
void connect_callback(uint16_t conn_handle) {
  output_debug_info("BLE connected");
  CS_CONNECTED = 1;

  // Get the reference to current connection
  BLEConnection* connection = Bluefruit.Connection(conn_handle);

  // request to update data length
  // connection->requestDataLengthUpdate();

  // request mtu exchange
  // connection->requestMtuExchange(MAX_PACKAGE_LEN);

  char central_name[32] = { 0 };
  connection->getPeerName(central_name, sizeof(central_name));

  output_debug_info("Connected");
}

void disconnect_callback(uint16_t conn_handle, uint8_t reason) {
  (void) conn_handle;
  (void) reason;

  CS_CONNECTED = 0;

  output_debug_info("BLE disconnected");
}

/*
void rx_callback(uint16_t conn_handle) {
  output_debug_info("BLE disconnected");
  Comm_update();
}
*/

void ant_comm_update(void) {
  if (bleuart.available()) {
    int recv_data_len = bleuart.available();
    output_debug_info_int("recv_data_len: ", recv_data_len);
  }
}

#ifdef __cplusplus
}
#endif


