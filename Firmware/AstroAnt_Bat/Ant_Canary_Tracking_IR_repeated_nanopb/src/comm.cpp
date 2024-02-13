#include <Arduino.h>

#include "comm.h"
#include "system.h"
#include "ant_canary_tracking.h"
#include "ant_canary_IR.h"

volatile bool bleConnected = false;

ANT_CANARY_RECV_CMD ant_canary_recv_cmd(CMD_RECV_INVAID, 0);

uint8_t cmd_buffer[150] = {0};
uint8_t ping_buffer[150] = {0};
uint8_t IR_buffer[200] = {0};

uint32_t seq_num_i = 0;

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
  connection->requestMtuExchange(200);

  char central_name[32] = { 0 };
  connection->getPeerName(central_name, sizeof(central_name));

  digitalWrite(BLE_LED_PIN, HIGH);
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

  digitalWrite(BLE_LED_PIN, LOW);
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
  Bluefruit.setName(ANT_NAME);

  // Configure and Start BLE Uart Service
  bleuart.begin();

  // Set up and start advertising
  startAdv();

  flash_led(MSG_LED_PIN, 3, 100);
}

int decodecmd(const uint8_t* data, size_t size) {
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
    return 0;
  }

  ant_canary_recv_cmd.recv_cmd  = Ant_cmd_frame_recv.cmd_recv;
  ant_canary_recv_cmd.parameter = Ant_cmd_frame_recv.dis_ang;

  return 1;
}

int get_recv_cmd(size_t size) {
  for (unsigned int i = 0; i < size; i++) {
    cmd_buffer[i] = (uint8_t) bleuart.read();
  }

  flash_led(MSG_LED_PIN, 1, 50);

  if (!decodecmd(cmd_buffer, size)) {
    flash_led(MSG_LED_PIN, 3, 50);
  }

  if (SERIAL_DEBUG) {
    Serial.print("Received cmd:");
    Serial.println(ant_canary_recv_cmd.recv_cmd);
  }

  return 1;
  // return CMD_RECV_INVAID;
}

void send_ack(_CMD_RECV cmd_recv, _ACK2SEND ack2send) {
  Ant_ack_frame Ant_ack_frame_send = Ant_ack_frame_init_zero;
  pb_ostream_t stream_send = pb_ostream_from_buffer(cmd_buffer, sizeof(cmd_buffer));

  Ant_ack_frame_send.cmd_recv = cmd_recv;
  Ant_ack_frame_send.ack2send = ack2send;
  Ant_ack_frame_send.crc      = cmd_recv + ack2send;

  bool status = pb_encode(&stream_send, Ant_ack_frame_fields, &Ant_ack_frame_send);

  if (SERIAL_DEBUG) {
    if (!status) {
      Serial.println("Encoding failed");
      return;
    }
  }

  if (SERIAL_DEBUG) {
    Serial.println("Sending ack:");
    for (unsigned int i = 0; i < stream_send.bytes_written; i++) {
      Serial.print(cmd_buffer[i], HEX);
      Serial.print(" ");
    }
    Serial.println("");
  }

  bleuart.write(cmd_buffer, stream_send.bytes_written);
  flash_led(MSG_LED_PIN, 1, 100);
}

void handle_cmd(ANT_CANARY_RECV_CMD * cmd_recv) {
  seq_num_i = 0;

  if (cmd_recv->recv_cmd == CMD_RECV_START) {
    // TODO: start track moving
    flash_led(MSG_LED_PIN, 1, 50);
    send_ack(CMD_RECV_START, ACK2SEND_ACK);
    start_track_moving();
  }
  if (cmd_recv->recv_cmd == CMD_RECV_FORWARD) {
    // TODO: send ack data back
    flash_led(MSG_LED_PIN, 1, 50);
    send_ack(CMD_RECV_FORWARD, ACK2SEND_ACK);
    move_forward(cmd_recv->parameter);
  }
  if (cmd_recv->recv_cmd == CMD_RECV_BACKWARD) {
    // TODO: send ack data back
    flash_led(MSG_LED_PIN, 1, 50);
    send_ack(CMD_RECV_BACKWARD, ACK2SEND_ACK);
    move_backward(cmd_recv->parameter);
  }
  if (cmd_recv->recv_cmd == CMD_RECV_TURN_L) {
    // TODO: start track moving
    flash_led(MSG_LED_PIN, 1, 50);
    send_ack(CMD_RECV_TURN_L, ACK2SEND_ACK);
    turn_left(cmd_recv->parameter);
  }
  if (cmd_recv->recv_cmd == CMD_RECV_TURN_R) {
    // TODO: start track moving
    flash_led(MSG_LED_PIN, 1, 50);
    send_ack(CMD_RECV_TURN_R, ACK2SEND_ACK);
    turn_right(cmd_recv->parameter);
  }
  if (cmd_recv->recv_cmd == CMD_RECV_PING) {
    // TODO: send ack data back
    flash_led(MSG_LED_PIN, 1, 50);
    send_ack(CMD_RECV_PING, ACK2SEND_ACK);
    ping_ack();
  }
  if (cmd_recv->recv_cmd == CMD_RECV_TAKEIR) {
    // TODO: take IR image
    flash_led(MSG_LED_PIN, 1, 50);
    send_ack(CMD_RECV_TAKEIR, ACK2SEND_ACK);
    take_ir_image();
  }
  if (cmd_recv->recv_cmd == CMD_RECV_INVAID) {
    flash_led(MSG_LED_PIN, 3, 50);
    send_ack(CMD_RECV_INVAID, ACK2SEND_NCK);
    // take_ir_image();
    // start_track_moving();
    // ping_ack();
  }
}

float get_gyro_data() {
  mpu.getEvent(&mpu_a, &mpu_g, &mpu_temp);

  return mpu_g.gyro.z;
}

void ping_ack(void) {
  Ant_tracking_data_frame tracking_data_frame = Ant_tracking_data_frame_init_zero;
  pb_ostream_t   stream         = pb_ostream_from_buffer(ping_buffer, sizeof(ping_buffer));

  tracking_data_frame.ID          = ANT_ID;
  tracking_data_frame.time_stamp  = millis();
  tracking_data_frame.seq_num     = seq_num_i;

  tracking_data_frame.battery_v   = read_bat_V();
  tracking_data_frame.encoder_cnt = encoder1Counter/256;
  tracking_data_frame.gyro_data   = get_gyro_data() - gyro_z_bias;

  int32_t crc_i = tracking_data_frame.ID
                + tracking_data_frame.time_stamp
                + tracking_data_frame.seq_num
                + tracking_data_frame.battery_v
                + tracking_data_frame.encoder_cnt
                + tracking_data_frame.gyro_data;

  tracking_data_frame.crc         = crc_i;

  pb_encode(&stream, Ant_tracking_data_frame_fields, &tracking_data_frame);
  size_t message_length = stream.bytes_written;

  if (SERIAL_DEBUG) {
    Serial.print("message_length: ");
    Serial.println(message_length);
    // print the whole data_buffer
    for (size_t i = 0; i < message_length; i++) {
      Serial.print(ping_buffer[i], HEX);
      Serial.print(" ");
    }
  }

  seq_num_i++;

  bleuart.write(ping_buffer, message_length);
}

void IR_ack(void) {
  // send data colum by colum
  for (uint8_t w=0; w<32; w++) {
    Ant_IR_data_frame IR_data_frame = Ant_IR_data_frame_init_zero;
    pb_ostream_t   stream           = pb_ostream_from_buffer(IR_buffer, sizeof(IR_buffer));

    IR_data_frame.ID      = ANT_ID;
    IR_data_frame.seq_num = seq_num_i;

    int32_t crc_i = IR_data_frame.ID + IR_data_frame.seq_num;

    for (uint8_t h=0; h<24; h++) {
      float temp = mlx90640To[h*32 + w];
      IR_data_frame.IR_data[h] = temp;
      crc_i += temp;
    }

    // for (uint8_t h=0; h<24; h++) {
    //   float temp = mlx90640To[h*32 + w];
    //   crc_i += temp;

    //   // TODO: how to set value to repeated field?
    //   // IR_data_frame.IR_data[h] = &mlx90640To[h*32 + w];
    // }

    IR_data_frame.crc = crc_i;

    pb_encode(&stream, Ant_IR_data_frame_fields, &IR_data_frame);
    size_t message_length = stream.bytes_written;

    output_debug_info_int("IR message_length: ", message_length);

    seq_num_i++;

    bleuart.write(IR_buffer, message_length);
  }
}