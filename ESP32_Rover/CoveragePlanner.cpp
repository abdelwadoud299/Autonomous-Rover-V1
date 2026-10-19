
#include "CoveragePlanner.h"
#include "Config.h"
#include "MotionControl.h"
#include "Encoder.h"
#include <math.h>

enum PlannerState
{
    PLANNER_IDLE,
    PLANNER_MOVING_FORWARD,
    PLANNER_TURNING_TO_NEXT_ROW,
    PLANNER_SHIFTING_ROW,
    PLANNER_TURNING_TO_ROW,
    PLANNER_PAUSED,
    PLANNER_FINISHED
};

static PlannerState plannerState = PLANNER_IDLE;
static PlannerState pausedState = PLANNER_IDLE;

static float fieldWidthM = 0.0f;
static float rowLengthM = 0.0f;
static float rowSpacingM = 0.0f;

static uint8_t currentRow = 0;
static uint8_t totalRows = 0;

static bool forwardDirection = true;
static bool turnRight = true;

static float startLeftCM = 0.0f;
static float startRightCM = 0.0f;

static void SaveEncoderPosition(void)
{
    startLeftCM = Encoder_GetLeftDistanceCM();
    startRightCM = Encoder_GetRightDistanceCM();
}

static float GetAverageTravelCM(void)
{
    float leftDelta =
        Encoder_GetLeftDistanceCM() - startLeftCM;

    float rightDelta =
        Encoder_GetRightDistanceCM() - startRightCM;

    return (fabsf(leftDelta) + fabsf(rightDelta)) / 2.0f;
}

static float GetTurnDifferenceCM(void)
{
    float leftDelta =
        Encoder_GetLeftDistanceCM() - startLeftCM;

    float rightDelta =
        Encoder_GetRightDistanceCM() - startRightCM;

    return fabsf(leftDelta - rightDelta);
}

static void ApplyCurrentMotion(void)
{
    switch (plannerState)
    {
        case PLANNER_MOVING_FORWARD:
            if (forwardDirection)
                Motion_Forward(COVERAGE_SPEED);
            else
                Motion_Backward(COVERAGE_SPEED);
            break;

        case PLANNER_TURNING_TO_NEXT_ROW:
        case PLANNER_TURNING_TO_ROW:
            if (turnRight)
                Motion_TurnRight(COVERAGE_SPEED);
            else
                Motion_TurnLeft(COVERAGE_SPEED);
            break;

        case PLANNER_SHIFTING_ROW:
            Motion_Forward(COVERAGE_SPEED);
            break;

        default:
            Motion_Stop();
            break;
    }
}

static void StartTurn(void)
{
    SaveEncoderPosition();

    if (turnRight)
        Motion_TurnRight(COVERAGE_SPEED);
    else
        Motion_TurnLeft(COVERAGE_SPEED);
}

void Coverage_Init(float widthM, float lengthM)
{
    if (widthM <= 0.0f ||
        lengthM <= 0.0f ||
        COVERAGE_ROW_WIDTH_M <= 0.0f)
    {
        totalRows = 0;
        plannerState = PLANNER_IDLE;
        pausedState = PLANNER_IDLE;
        return;
    }

    fieldWidthM = widthM;
    rowLengthM = lengthM;
    rowSpacingM = COVERAGE_ROW_WIDTH_M;

    totalRows = (uint8_t)ceilf(fieldWidthM / rowSpacingM);

    currentRow = 0;
    forwardDirection = true;
    turnRight = true;

    plannerState = PLANNER_IDLE;
    pausedState = PLANNER_IDLE;
}

void Coverage_Start(void)
{
    if (totalRows == 0)
        return;

    currentRow = 0;
    forwardDirection = true;
    turnRight = true;

    Encoder_Reset();
    SaveEncoderPosition();

    plannerState = PLANNER_MOVING_FORWARD;
    pausedState = PLANNER_IDLE;

    Motion_Forward(COVERAGE_SPEED);
}

void Coverage_Stop(void)
{
    plannerState = PLANNER_IDLE;
    pausedState = PLANNER_IDLE;
    Motion_Stop();
}

void Coverage_Pause(void)
{
    if (plannerState == PLANNER_IDLE ||
        plannerState == PLANNER_FINISHED ||
        plannerState == PLANNER_PAUSED)
    {
        return;
    }

    pausedState = plannerState;
    plannerState = PLANNER_PAUSED;

    Motion_Stop();
}

void Coverage_Resume(void)
{
    if (plannerState != PLANNER_PAUSED)
        return;

    plannerState = pausedState;
    pausedState = PLANNER_IDLE;

    SaveEncoderPosition();
    ApplyCurrentMotion();
}

void Coverage_Update(void)
{
    if (plannerState == PLANNER_IDLE ||
        plannerState == PLANNER_PAUSED ||
        plannerState == PLANNER_FINISHED)
    {
        return;
    }

    float averageTravelCM = GetAverageTravelCM();

    switch (plannerState)
    {
        case PLANNER_MOVING_FORWARD:

            if (averageTravelCM >= rowLengthM * 100.0f)
            {
                Motion_Stop();

                currentRow++;

                if (currentRow >= totalRows)
                {
                    plannerState = PLANNER_FINISHED;
                    return;
                }

                turnRight = forwardDirection;
                StartTurn();

                plannerState = PLANNER_TURNING_TO_NEXT_ROW;
            }
            break;

        case PLANNER_TURNING_TO_NEXT_ROW:

            if (GetTurnDifferenceCM() >=
                WHEEL_BASE_CM * 1.5707963f)
            {
                Motion_Stop();

                SaveEncoderPosition();
                Motion_Forward(COVERAGE_SPEED);

                plannerState = PLANNER_SHIFTING_ROW;
            }
            break;

        case PLANNER_SHIFTING_ROW:

            if (averageTravelCM >= rowSpacingM * 100.0f)
            {
                Motion_Stop();

                turnRight = !turnRight;
                StartTurn();

                plannerState = PLANNER_TURNING_TO_ROW;
            }
            break;

        case PLANNER_TURNING_TO_ROW:

            if (GetTurnDifferenceCM() >=
                WHEEL_BASE_CM * 1.5707963f)
            {
                Motion_Stop();

                forwardDirection = !forwardDirection;

                SaveEncoderPosition();

                plannerState = PLANNER_MOVING_FORWARD;
                ApplyCurrentMotion();
            }
            break;

        default:
            Motion_Stop();
            plannerState = PLANNER_IDLE;
            break;
    }
}
