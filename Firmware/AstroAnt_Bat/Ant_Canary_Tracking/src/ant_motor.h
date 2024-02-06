#pragma once

#include <Arduino.h>

extern void motor_setup(void);

#define MOTOR_1_PIN_1 34
#define MOTOR_1_PIN_2 PIN_LED1

#define MOTOR_2_PIN_1 28
#define MOTOR_2_PIN_2 30

#define Encoder_A1 A6
#define Encoder_A2 5

extern volatile uint16_t encoder1Counter;
extern volatile uint16_t encoder2Counter;

extern void motor_setup(void);

extern void move_forward(int speed);
extern void move_backward(int speed);
extern void motor_tst(int speed);
extern void motor_stop(void);