#include "ant_canary_tracking.h"
#include "system.h"
#include "ant_canary_mpu6050.h"

// #ifdef __cplusplus
// extern "C"
// {
// #endif // __cplusplus

void start_track_moving(void) {
  move_forward(100);
  turn_right(30);
  move_forward(100);
  turn_left(120);
  move_backward(100);
}

void move_forward(int distance) {
  IMU_cali();
  ;
}
void move_backward(int distance) {
  IMU_cali();
  ;
}

void turn_left(int angle) {
  IMU_cali();
  ;
}
void turn_right(int angle) {
  IMU_cali();
  ;
}

// #ifdef __cplusplus
// }
// #endif