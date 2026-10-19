
#ifndef MOTION_CONTROL_H
#define MOTION_CONTROL_H

#include <Arduino.h>

void Motion_Init(void);

void Motion_Forward(int16_t speed);
void Motion_Backward(int16_t speed);

void Motion_TurnLeft(int16_t speed);
void Motion_TurnRight(int16_t speed);

void Motion_Stop(void);

#endif
