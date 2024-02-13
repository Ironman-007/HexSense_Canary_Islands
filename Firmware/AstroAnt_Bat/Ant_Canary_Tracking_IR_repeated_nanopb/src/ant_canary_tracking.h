#include <Arduino.h>

#include "comm.h"
#include "ant_motor.h"
#include "ant_canary_mpu6050.h"

extern void start_track_moving(void);

extern void move_forward(int distance);
extern void move_backward(int distance);

extern void turn_left(int angle);
extern void turn_right(int angle);
