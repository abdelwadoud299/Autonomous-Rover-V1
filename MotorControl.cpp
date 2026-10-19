
#include "MotorControl.h"
#include "Config.h"

static int8_t leftDirection = 0;
static int8_t rightDirection = 0;

static void Motor_SetSpeed(
    uint8_t pin1,
    uint8_t pin2,
    int16_t speed,
    int8_t &currentDirection)
{
    speed = constrain(speed, -255, 255);

    int8_t newDirection = (speed > 0) ? 1 :
                          (speed < 0) ? -1 : 0;

    // Stop before reversing direction
    if (newDirection != 0 &&
        currentDirection != 0 &&
        newDirection != currentDirection)
    {
        analogWrite(pin1, 0);
        analogWrite(pin2, 0);
        delay(50);
    }

    uint8_t pwm = abs(speed);

    if (speed > 0)
    {
        analogWrite(pin1, pwm);
        analogWrite(pin2, 0);
    }
    else if (speed < 0)
    {
        analogWrite(pin1, 0);
        analogWrite(pin2, pwm);
    }
    else
    {
        analogWrite(pin1, 0);
        analogWrite(pin2, 0);
    }

    currentDirection = newDirection;
}

void Motor_Init(void)
{
    pinMode(MOTOR_LEFT_D0, OUTPUT);
    pinMode(MOTOR_LEFT_D1, OUTPUT);
    pinMode(MOTOR_RIGHT_D2, OUTPUT);
    pinMode(MOTOR_RIGHT_D3, OUTPUT);

    Motor_Stop();
}

void Motor_Left(int16_t speed)
{
    Motor_SetSpeed(
        MOTOR_LEFT_D0,
        MOTOR_LEFT_D1,
        speed,
        leftDirection
    );
}

void Motor_Right(int16_t speed)
{
    Motor_SetSpeed(
        MOTOR_RIGHT_D2,
        MOTOR_RIGHT_D3,
        speed,
        rightDirection
    );
}

void Motor_Stop(void)
{
    analogWrite(MOTOR_LEFT_D0, 0);
    analogWrite(MOTOR_LEFT_D1, 0);
    analogWrite(MOTOR_RIGHT_D2, 0);
    analogWrite(MOTOR_RIGHT_D3, 0);

    leftDirection = 0;
    rightDirection = 0;
}
