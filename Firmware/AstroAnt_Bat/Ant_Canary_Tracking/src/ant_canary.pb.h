/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.8 */

#ifndef PB_ANT_CANARY_PB_H_INCLUDED
#define PB_ANT_CANARY_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
typedef enum _CMD_RECV {
    CMD_RECV_START = 0,
    CMD_RECV_FORWARD = 1,
    CMD_RECV_BACKWARD = 2,
    CMD_RECV_TURN_L = 3,
    CMD_RECV_TURN_R = 4,
    CMD_RECV_PING = 5,
    CMD_RECV_TAKEIR = 6,
    CMD_RECV_INVAID = 7
} CMD_RECV;

typedef enum _ACK2SEND {
    ACK2SEND_ACK = 0,
    ACK2SEND_NCK = 1
} ACK2SEND;

/* Struct definitions */
typedef struct _Ant_tracking_data_frame {
    int32_t time_stamp;
    int32_t seq_num;
    float battery_v;
    float encoder_cnt;
    float gyro_data;
    int32_t crc;
} Ant_tracking_data_frame;

typedef struct _Ant_IR_data_frame {
    int32_t time_stamp;
    int32_t seq_num;
    float battery_v;
    pb_callback_t IR_data;
    int32_t crc;
} Ant_IR_data_frame;

typedef struct _Ant_cmd_frame {
    int32_t ID;
    CMD_RECV cmd_recv;
    int32_t dis_ang;
    int32_t crc;
} Ant_cmd_frame;

typedef struct _Ant_ack_frame {
    int32_t ID;
    ACK2SEND ack2send;
    int32_t crc;
} Ant_ack_frame;


#ifdef __cplusplus
extern "C" {
#endif

/* Helper constants for enums */
#define _CMD_RECV_MIN CMD_RECV_START
#define _CMD_RECV_MAX CMD_RECV_INVAID
#define _CMD_RECV_ARRAYSIZE ((CMD_RECV)(CMD_RECV_INVAID+1))

#define _ACK2SEND_MIN ACK2SEND_ACK
#define _ACK2SEND_MAX ACK2SEND_NCK
#define _ACK2SEND_ARRAYSIZE ((ACK2SEND)(ACK2SEND_NCK+1))



#define Ant_cmd_frame_cmd_recv_ENUMTYPE CMD_RECV

#define Ant_ack_frame_ack2send_ENUMTYPE ACK2SEND


/* Initializer values for message structs */
#define Ant_tracking_data_frame_init_default     {0, 0, 0, 0, 0, 0}
#define Ant_IR_data_frame_init_default           {0, 0, 0, {{NULL}, NULL}, 0}
#define Ant_cmd_frame_init_default               {0, _CMD_RECV_MIN, 0, 0}
#define Ant_ack_frame_init_default               {0, _ACK2SEND_MIN, 0}
#define Ant_tracking_data_frame_init_zero        {0, 0, 0, 0, 0, 0}
#define Ant_IR_data_frame_init_zero              {0, 0, 0, {{NULL}, NULL}, 0}
#define Ant_cmd_frame_init_zero                  {0, _CMD_RECV_MIN, 0, 0}
#define Ant_ack_frame_init_zero                  {0, _ACK2SEND_MIN, 0}

/* Field tags (for use in manual encoding/decoding) */
#define Ant_tracking_data_frame_time_stamp_tag   1
#define Ant_tracking_data_frame_seq_num_tag      2
#define Ant_tracking_data_frame_battery_v_tag    3
#define Ant_tracking_data_frame_encoder_cnt_tag  4
#define Ant_tracking_data_frame_gyro_data_tag    5
#define Ant_tracking_data_frame_crc_tag          6
#define Ant_IR_data_frame_time_stamp_tag         1
#define Ant_IR_data_frame_seq_num_tag            2
#define Ant_IR_data_frame_battery_v_tag          3
#define Ant_IR_data_frame_IR_data_tag            4
#define Ant_IR_data_frame_crc_tag                5
#define Ant_cmd_frame_ID_tag                     1
#define Ant_cmd_frame_cmd_recv_tag               2
#define Ant_cmd_frame_dis_ang_tag                3
#define Ant_cmd_frame_crc_tag                    4
#define Ant_ack_frame_ID_tag                     1
#define Ant_ack_frame_ack2send_tag               2
#define Ant_ack_frame_crc_tag                    3

/* Struct field encoding specification for nanopb */
#define Ant_tracking_data_frame_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    time_stamp,        1) \
X(a, STATIC,   SINGULAR, INT32,    seq_num,           2) \
X(a, STATIC,   SINGULAR, FLOAT,    battery_v,         3) \
X(a, STATIC,   SINGULAR, FLOAT,    encoder_cnt,       4) \
X(a, STATIC,   SINGULAR, FLOAT,    gyro_data,         5) \
X(a, STATIC,   SINGULAR, INT32,    crc,               6)
#define Ant_tracking_data_frame_CALLBACK NULL
#define Ant_tracking_data_frame_DEFAULT NULL

#define Ant_IR_data_frame_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    time_stamp,        1) \
X(a, STATIC,   SINGULAR, INT32,    seq_num,           2) \
X(a, STATIC,   SINGULAR, FLOAT,    battery_v,         3) \
X(a, CALLBACK, REPEATED, FLOAT,    IR_data,           4) \
X(a, STATIC,   SINGULAR, INT32,    crc,               5)
#define Ant_IR_data_frame_CALLBACK pb_default_field_callback
#define Ant_IR_data_frame_DEFAULT NULL

#define Ant_cmd_frame_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    ID,                1) \
X(a, STATIC,   SINGULAR, UENUM,    cmd_recv,          2) \
X(a, STATIC,   SINGULAR, INT32,    dis_ang,           3) \
X(a, STATIC,   SINGULAR, INT32,    crc,               4)
#define Ant_cmd_frame_CALLBACK NULL
#define Ant_cmd_frame_DEFAULT NULL

#define Ant_ack_frame_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    ID,                1) \
X(a, STATIC,   SINGULAR, UENUM,    ack2send,          2) \
X(a, STATIC,   SINGULAR, INT32,    crc,               3)
#define Ant_ack_frame_CALLBACK NULL
#define Ant_ack_frame_DEFAULT NULL

extern const pb_msgdesc_t Ant_tracking_data_frame_msg;
extern const pb_msgdesc_t Ant_IR_data_frame_msg;
extern const pb_msgdesc_t Ant_cmd_frame_msg;
extern const pb_msgdesc_t Ant_ack_frame_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define Ant_tracking_data_frame_fields &Ant_tracking_data_frame_msg
#define Ant_IR_data_frame_fields &Ant_IR_data_frame_msg
#define Ant_cmd_frame_fields &Ant_cmd_frame_msg
#define Ant_ack_frame_fields &Ant_ack_frame_msg

/* Maximum encoded size of messages (where known) */
/* Ant_IR_data_frame_size depends on runtime parameters */
#define ANT_CANARY_PB_H_MAX_SIZE                 Ant_tracking_data_frame_size
#define Ant_ack_frame_size                       24
#define Ant_cmd_frame_size                       35
#define Ant_tracking_data_frame_size             48

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
