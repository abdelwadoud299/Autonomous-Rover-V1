
#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H

#include <Arduino.h>

void RobotController_Init(void);
void RobotController_StartCoverage(void);
void RobotController_Stop(void);
void RobotController_Update(void);

#endif
