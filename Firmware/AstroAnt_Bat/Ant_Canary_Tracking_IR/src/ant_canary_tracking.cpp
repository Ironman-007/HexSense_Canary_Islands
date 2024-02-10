#include "ant_canary_tracking.h"
#include "system.h"
#include "ant_canary_mpu6050.h"

// #ifdef __cplusplus
// extern "C"
// {
// #endif // __cplusplus

void start_track_moving(void) {
  move_forward(15);
  turn_right(5);
  move_forward(15);
  turn_left(16);
  move_forward(15);
  move_backward(10);
}

void move_forward(int distance) {
  IMU_cali();

  int _running_time = 0;

  forward(240);

  while (_running_time < distance) {
    if (timer4Interrupt_100ms) {
      timer4Interrupt_100ms = false;

      ping_ack();

      _running_time ++;
    }
  }

  stop();
}

void move_backward(int distance) {
  IMU_cali();

  int _running_time = 0;

  backward(240);

  while (_running_time < distance) {
    if (timer4Interrupt_100ms) {
      timer4Interrupt_100ms = false;

      ping_ack();

      _running_time ++;
    }
  }
  stop();
}

void turn_left(int angle) {
  IMU_cali();

  int _running_time = 0;

  left(240);

  while (_running_time < angle) {
    if (timer4Interrupt_100ms) {
      timer4Interrupt_100ms = false;

      ping_ack();

      _running_time ++;
    }
  }
  stop();
}
void turn_right(int angle) {
  IMU_cali();

  int _running_time = 0;

  right(240);

  while (_running_time < angle) {
    if (timer4Interrupt_100ms) {
      timer4Interrupt_100ms = false;

      ping_ack();

      _running_time ++;
    }
  }
  stop();
}

// #ifdef __cplusplus
// }
// #endif