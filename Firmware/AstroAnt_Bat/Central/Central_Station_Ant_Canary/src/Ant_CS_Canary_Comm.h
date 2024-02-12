#pragma once

#include <Arduino.h>

#include <pb_encode.h>
#include <pb_decode.h>

#include "ant_canary.pb.h"

extern void send_cmd(CMD_RECV cmd2send);

extern void print_recv_data(void);