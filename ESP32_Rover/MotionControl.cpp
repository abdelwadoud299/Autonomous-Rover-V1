
#include "MotionControl.h"
#include "MotorControl.h"

void Motion_Init(void)
{
    Motor_Init();
}

void Motion_Forward(int16_t speed)
{
    Motor_Left(speed);
    Motor_Right(speed);
}

void Motion_Backward(int16_t speed)
{
    Motor_Left(-speed);
    Motor_Right(-speed);
}

void Motion_TurnLeft(int16_t speed)
{
    Motor_Left(-speed);
    Motor_Right(speed);
}

void Motion_TurnRight(int16_t speed)
{
    Motor_Left(speed);
    Motor_Right(-speed);
}

void Motion_Stop(void)
{
    Motor_Stop();
}
