
#ifndef COVERAGE_PLANNER_H
#define COVERAGE_PLANNER_H

#include <Arduino.h>

void Coverage_Init(float fieldWidthM, float rowLengthM);
void Coverage_Start(void);
void Coverage_Stop(void);
void Coverage_Pause(void);
void Coverage_Resume(void);
void Coverage_Update(void);

#endif
