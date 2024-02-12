#include <Arduino.h>
#include <bluefruit.h>

#include "Ant_CS_Canary_Comm.h"

BLEClientUart clientUart; // bleuart client


/**
 * Callback invoked when scanner pick up an advertising data
 * @param report Structural advertising data
 */
void scan_callback(ble_gap_evt_adv_report_t* report) 
{
  // Check if advertising contain BleUart service
  if ( Bluefruit.Scanner.checkReportForService(report, clientUart) )
  {
    Serial.print("BLE UART service detected. Connecting ... ");

    // Connect to device with bleuart service in advertising
    Bluefruit.Central.connect(report);
  } else
  {      
    // For Softdevice v6: after received a report, scanner will be paused
    // We need to call Scanner resume() to continue scanning
    Bluefruit.Scanner.resume();
  }
}

/**
 * Callback invoked when an connection is established
 * @param conn_handle
 */
void connect_callback(uint16_t conn_handle)
{
  Serial.println("Connected");

  Serial.print("Discovering BLE Uart Service ... ");
  if ( clientUart.discover(conn_handle) )
  {
    Serial.println("Found it");

    Serial.println("Enable TXD's notify");
    clientUart.enableTXD();

    Serial.println("Ready to receive from peripheral");
  } else
  {
    Serial.println("Found NONE");
    
    // disconnect since we couldn't find bleuart service
    Bluefruit.disconnect(conn_handle);
  }  
}

/**
 * Callback invoked when a connection is dropped
 * @param conn_handle
 * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
 */
void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;
  
  Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
}

/**
 * Callback invoked when uart received data
 * @param uart_svc Reference object to the service where the data 
 * arrived. In this example it is clientUart
 */
void bleuart_rx_callback(BLEClientUart& uart_svc)
{
  Serial.print("[RX]: ");

  while ( uart_svc.available() )
  {
    Serial.print(uart_svc.read(), HEX);
    Serial.print(" ");
    // Serial.print( (char) uart_svc.read() );
  }

  Serial.println();
}

void setup() {
  Serial.begin(115200);
  while ( !Serial ) delay(10);   // for nrf52840 with native usb

  Serial.println("Bluefruit52 Central BLEUART Example");
  Serial.println("-----------------------------------\n");

  // Initialize Bluefruit with maximum connections as Peripheral = 0, Central = 1
  // SRAM usage required by SoftDevice will increase dramatically with number of connections
  Bluefruit.begin(0, 1);
  Bluefruit.setName("Ant_Canary Central");

  // Init BLE Central Uart Serivce
  clientUart.begin();
  clientUart.setRxCallback(bleuart_rx_callback);

  // Increase Blink rate to different from PrPh advertising mode
  Bluefruit.setConnLedInterval(250);

  // Callbacks for Central
  Bluefruit.Central.setConnectCallback(connect_callback);
  Bluefruit.Central.setDisconnectCallback(disconnect_callback);
  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.setInterval(160, 80); // in unit of 0.625 ms
  Bluefruit.Scanner.useActiveScan(false);
  Bluefruit.Scanner.start(0);             // 0 = Don't stop scanning after n seconds

  Serial.print("CMD_RECV_START: ");
  send_cmd(CMD_RECV_START);
  delay(100);
  Serial.print("CMD_RECV_FORWARD: ");
  send_cmd(CMD_RECV_FORWARD);
  delay(100);
  Serial.print("CMD_RECV_BACKWARD: ");
  send_cmd(CMD_RECV_BACKWARD);
  delay(100);
  Serial.print("CMD_RECV_TURN_L: ");
  send_cmd(CMD_RECV_TURN_L);
  delay(100);
  Serial.print("CMD_RECV_TURN_R: ");
  send_cmd(CMD_RECV_TURN_R);
  delay(100);
  Serial.print("CMD_RECV_PING: ");
  send_cmd(CMD_RECV_PING);
  delay(100);
  Serial.print("CMD_RECV_TAKEIR: ");
  send_cmd(CMD_RECV_TAKEIR);
  delay(100);
  Serial.print("CMD_RECV_INVAID: ");
  send_cmd(CMD_RECV_INVAID);
  delay(100);
}

const uint8_t START_CMD[4]    = {0x10, 0x0A, 0x18, 0x0A};
const uint8_t FORWARD_CMD[6]  = {0x08, 0x01, 0x10, 0x0A, 0x18, 0x0B};
const uint8_t BACKWARD_CMD[6] = {0x08, 0x02, 0x10, 0x0A, 0x18, 0x0C};
const uint8_t TURN_L_CMD[6]   = {0x08, 0x03, 0x10, 0x0A, 0x18, 0x0D};
const uint8_t TURN_R_CMD[6]   = {0x08, 0x04, 0x10, 0x0A, 0x18, 0x0E};
const uint8_t PING_CMD[6]     = {0x08, 0x05, 0x10, 0x0A, 0x18, 0x0F};
const uint8_t IR_CMD[6]       = {0x08, 0x06, 0x10, 0x0A, 0x18, 0x10};
const uint8_t INCAVLID_CMD[6] = {0x08, 0x07, 0x10, 0x0A, 0x18, 0x11};

void loop()
{
  if ( Bluefruit.Central.connected() )
  {
    // Not discovered yet
    if ( clientUart.discovered() )
    {
      // Discovered means in working state
      // Get Serial input and send to Peripheral
      if ( Serial.available() )
      {
        delay(2); // delay a bit for all characters to arrive
        uint8_t str[1+1] = {0};

        Serial.read(str, sizeof(str));

        switch (str[0])
        {
          case 's':
            // send_cmd(CMD_RECV_START);
            Serial.println("START_CMD");
            clientUart.write(START_CMD, sizeof(START_CMD));
            break;

          case 'f':
            Serial.println("FORWARD_CMD");
            clientUart.write(FORWARD_CMD, sizeof(FORWARD_CMD));
            break;

          case 'b':
            Serial.println("BACKWARD_CMD");
            clientUart.write(BACKWARD_CMD, sizeof(BACKWARD_CMD));
            break;

          case 'l':
            Serial.println("TURN_L_CMD");
            clientUart.write(TURN_L_CMD, sizeof(TURN_L_CMD));
            break;

          case 'r':
            Serial.println("TURN_R_CMD");
            clientUart.write(TURN_R_CMD, sizeof(TURN_R_CMD));
            break;

          case 'p':
            Serial.println("PING_CMD");
            clientUart.write(PING_CMD, sizeof(PING_CMD));
            break;

          case 'i':
            Serial.println("IR_CMD");
            clientUart.write(IR_CMD, sizeof(IR_CMD));
            break;

          case 'x':
            Serial.println("INCAVLID_CMD");
            clientUart.write(INCAVLID_CMD, sizeof(INCAVLID_CMD));
            break;

          default:
            Serial.println("INCAVLID_CMD");
            clientUart.write(INCAVLID_CMD, sizeof(INCAVLID_CMD));
            break;
        }
      }
    }
  }
}
