
#include "ObstacleAvoidance.h"
#include "MotionControl.h"
#include "RobotState.h"
#include "Config.h"
#include <UltrasonicSensorLibrary.h>

extern RobotState robot;
extern Ultrasonic ultrasonic;

enum ObstacleState
{
    OBSTACLE_CLEAR,
    OBSTACLE_REVERSE,
    OBSTACLE_TURN
};

static ObstacleState state = OBSTACLE_CLEAR;
static unsigned long stateStartTime = 0;

bool ObstacleAvoidance_IsBusy(void)
{
    return state != OBSTACLE_CLEAR;
}

void ObstacleAvoidance_Init(void)
{
    state = OBSTACLE_CLEAR;
    stateStartTime = 0;
}

void ObstacleAvoidance_Update(void)
{
    unsigned long now = millis();

    float distance = ultrasonic.readCM();

    robot.obstacle_distance_cm = distance;
    robot.obstacle_detected =
        (distance > 0 && distance <= OBSTACLE_DISTANCE_CM);

    switch (state)
    {
        case OBSTACLE_CLEAR:
            if (robot.obstacle_detected)
            {
                Motion_Stop();

                robot.status = STATUS_OBSTACLE;

                state = OBSTACLE_REVERSE;
                stateStartTime = now;
            }
            break;

        case OBSTACLE_REVERSE:
            Motion_Backward(OBSTACLE_REVERSE_SPEED);

            if (now - stateStartTime >= OBSTACLE_REVERSE_TIME_MS)
            {
                Motion_Stop();

                state = OBSTACLE_TURN;
                stateStartTime = now;

                Motion_TurnRight(OBSTACLE_TURN_SPEED);
            }
            break;

        case OBSTACLE_TURN:
            if (now - stateStartTime >= OBSTACLE_TURN_TIME_MS)
            {
                Motion_Stop();

                state = OBSTACLE_CLEAR;
                robot.obstacle_detected = false;
                robot.status = STATUS_RUNNING;
            }
            break;
    }
}
