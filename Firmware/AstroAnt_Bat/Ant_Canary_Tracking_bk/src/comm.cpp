#include <Arduino.h>

#include "comm.h"
#include "system.h"

volatile bool bleConnected = false;

uint8_t cmd_buffer[100] = {0};

BLEUart bleuart; // uart over ble

void startAdv(void) {
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include bleuart 128-bit uuid
  Bluefruit.Advertising.addService(bleuart);

  // Secondary Scan Response packet (optional)
  // Since there is no room for 'Name' in Advertising packet
  Bluefruit.ScanResponse.addName();
  
  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
}

// callback invoked when central connects
void connect_callback(uint16_t conn_handle) {
  bleConnected = true;

  // Get the reference to current connection
  BLEConnection* connection = Bluefruit.Connection(conn_handle);

  connection->requestDataLengthUpdate();

  // request mtu exchange
  connection->requestMtuExchange(200);

  char central_name[32] = { 0 };
  connection->getPeerName(central_name, sizeof(central_name));
}

/**
 * Callback invoked when a connection is dropped
 * @param conn_handle connection where this event happens
 * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
 */
void disconnect_callback(uint16_t conn_handle, uint8_t reason) {
  bleConnected = false;

  (void) conn_handle;
  (void) reason;
}

void ble_setup() {
  // Setup the BLE LED to be enabled on CONNECT
  // Note: This is actually the default behavior, but provided
  // here in case you want to control this LED manually via PIN 19
  Bluefruit.autoConnLed(true);

  // Config the peripheral connection with maximum bandwidth 
  // more SRAM required by SoftDevice
  // Note: All config***() function must be called before begin()
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);

  Bluefruit.begin();
  Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
  //Bluefruit.setName(getMcuUniqueID()); // useful testing with multiple central connections
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);
  Bluefruit.setName("AstroAnt-Lanzarote");

  // Configure and Start BLE Uart Service
  bleuart.begin();

  // Start BLE Battery Service
//   blebas.begin();
//   blebas.write(100);

  // Set up and start advertising
  startAdv();
}

_CMD_RECV decodecmd(const uint8_t* data, size_t size) {
  Ant_cmd_frame Ant_cmd_frame_recv = Ant_cmd_frame_init_zero;
  pb_istream_t  stream_recv        = pb_istream_from_buffer(data, size);

  if (SERIAL_DEBUG) {
    Serial.println("Received cmd:");

    for (unsigned int i = 0; i < size; i++) {
      Serial.print(data[i], HEX);
      Serial.print(" ");
    }
  }
  Serial.println("");

  bool status = pb_decode(&stream_recv, Ant_cmd_frame_fields, &Ant_cmd_frame_recv);

  if (!status) {
    Serial.println("Decoding failed");
    return CMD_RECV_INVAID;
  }

  return Ant_cmd_frame_recv.cmd_recv;
}

_CMD_RECV get_recv_cmd(size_t size) {
  for (unsigned int i = 0; i < size; i++) {
    cmd_buffer[i] = (uint8_t) bleuart.read();
  }
  _CMD_RECV cmd_recv = decodecmd(cmd_buffer, size);

  Serial.print("Received cmd:");
  Serial.println(cmd_recv);

  return cmd_recv;
}

void handle_cmd(_CMD_RECV cmd_recv) {
  if (cmd_recv == CMD_RECV_START) {
    // TODO: start track moving 
  }
  if (cmd_recv == CMD_RECV_FORWARD) {
    // TODO: send ack data back
  }
  if (cmd_recv == CMD_RECV_BACKWARD) {
    // TODO: send ack data back
  }
  if (cmd_recv == CMD_RECV_TURN_L) {
    // TODO: start track moving 
  }
  if (cmd_recv == CMD_RECV_TURN_R) {
    // TODO: start track moving 
  }
  if (cmd_recv == CMD_RECV_PING) {
    // TODO: send ack data back
  }
  if (cmd_recv == CMD_RECV_TAKEIR) {
    // TODO: take IR image
  }
  if (cmd_recv == CMD_RECV_INVAID) {
  }
}
