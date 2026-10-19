#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

void Encoder_Init(void);

int32_t Encoder_GetLeftCount(void);
int32_t Encoder_GetRightCount(void);
float Encoder_GetLeftDistanceCM(void);
float Encoder_GetRightDistanceCM(void);
void Encoder_Reset(void);

#endif