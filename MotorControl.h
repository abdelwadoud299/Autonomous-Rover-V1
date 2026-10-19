#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <Arduino.h>

void Motor_Init(void);
void Motor_Left(int16_t speed);
void Motor_Right(int16_t speed);
void Motor_Stop(void);

#endif