#include "ant_motor.h"
#include "system.h"

volatile uint16_t encoder1Counter = 0;
volatile uint16_t encoder2Counter = 0;

void ENC1_INT_Routine() {
  encoder1Counter++;
}

void ENC2_INT_Routine() { 
  encoder2Counter++;
}

void motor_setup(void) {
  pinMode(Encoder_A1, INPUT);
  pinMode(Encoder_A2, INPUT);

  attachInterrupt(Encoder_A1, ENC1_INT_Routine, RISING);
  attachInterrupt(Encoder_A2, ENC2_INT_Routine, RISING);

  pinMode(MOTOR_1_PIN_1, OUTPUT);
  pinMode(MOTOR_1_PIN_2, OUTPUT);
  pinMode(MOTOR_2_PIN_1, OUTPUT);
  pinMode(MOTOR_2_PIN_2, OUTPUT);

  digitalWrite(MOTOR_1_PIN_1, LOW);
  digitalWrite(MOTOR_1_PIN_2, LOW);
  digitalWrite(MOTOR_2_PIN_1, LOW);
  digitalWrite(MOTOR_2_PIN_2, LOW);

  output_debug_info("Motor setup done");
}

void motor_tst(int speed) {
  analogWrite(MOTOR_1_PIN_1, speed);
  analogWrite(MOTOR_1_PIN_2, 0);
  analogWrite(MOTOR_2_PIN_1, 0);
  analogWrite(MOTOR_2_PIN_2, speed);
}

void move_forward(int speed) {
  analogWrite(MOTOR_1_PIN_1, speed);
  analogWrite(MOTOR_1_PIN_2, 0);
  analogWrite(MOTOR_2_PIN_1, 0);
  analogWrite(MOTOR_2_PIN_2, speed);
}

void move_backward(int speed) {
  analogWrite(MOTOR_1_PIN_1, 0);
  analogWrite(MOTOR_1_PIN_2, speed);
  analogWrite(MOTOR_2_PIN_1, speed);
  analogWrite(MOTOR_2_PIN_2, 0);
}

void motor_stop(void) {
  analogWrite(MOTOR_1_PIN_1, 255);
  analogWrite(MOTOR_1_PIN_2, 255);
  analogWrite(MOTOR_2_PIN_1, 255);
  analogWrite(MOTOR_2_PIN_2, 255);
}

