
#include "RobotController.h"
#include "CoveragePlanner.h"
#include "ObstacleAvoidance.h"
#include "MotionControl.h"
#include "Config.h"
#include "RobotState.h"

extern RobotState robot;

static bool coverageStarted = false;
static bool obstacleWasBusy = false;

void RobotController_Init(void)
{
    coverageStarted = false;
    obstacleWasBusy = false;

    Motion_Stop();
    ObstacleAvoidance_Init();
    Coverage_Init(FIELD_WIDTH_M, FIELD_LENGTH_M);

    robot.mode = MODE_IDLE;
    robot.status = STATUS_IDLE;
    robot.obstacle_detected = false;
    robot.obstacle_distance_cm = 0.0f;
}

void RobotController_StartCoverage(void)
{
    if (coverageStarted)
    {
        return;
    }

    Coverage_Start();
    coverageStarted = true;
    obstacleWasBusy = false;

    robot.mode = MODE_COVERAGE;
    robot.status = STATUS_RUNNING;
}

void RobotController_Stop(void)
{
    Coverage_Stop();
    Motion_Stop();

    coverageStarted = false;
    obstacleWasBusy = false;

    robot.mode = MODE_IDLE;
    robot.status = STATUS_IDLE;
}


void RobotController_Update(void)
{
    ObstacleAvoidance_Update();

    bool obstacleBusy = ObstacleAvoidance_IsBusy();

    if (obstacleBusy)
    {
        if (coverageStarted && !obstacleWasBusy)
        {
            Coverage_Pause();

            if (robot.obstacle_detected)
            {
                Motion_Backward(OBSTACLE_REVERSE_SPEED);
            }
        }

        obstacleWasBusy = true;
        return;
    }

    if (obstacleWasBusy)
    {
        obstacleWasBusy = false;

        if (coverageStarted)
        {
            Coverage_Resume();
            robot.status = STATUS_RUNNING;
        }

        return;
    }

    if (coverageStarted)
    {
        Coverage_Update();
    }
}
