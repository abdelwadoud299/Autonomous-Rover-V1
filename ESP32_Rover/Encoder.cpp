#include "Encoder.h"
#include "Config.h"
#include <ESP32Encoder.h>

static ESP32Encoder leftEncoder;
static ESP32Encoder rightEncoder;

void Encoder_Init(void)
{
    ESP32Encoder::useInternalWeakPullResistors = puType::up;

    leftEncoder.attachFullQuad(LEFT_ENCODER_A, LEFT_ENCODER_B);
    rightEncoder.attachFullQuad(RIGHT_ENCODER_A, RIGHT_ENCODER_B);

    leftEncoder.clearCount();
    rightEncoder.clearCount();
}
float Encoder_GetLeftDistanceCM(void)
{
    return (Encoder_GetLeftCount() * WHEEL_CIRCUMFERENCE_CM)
           / ENCODER_COUNTS_PER_OUTPUT_REV;
}

float Encoder_GetRightDistanceCM(void)
{
    return (Encoder_GetRightCount() * WHEEL_CIRCUMFERENCE_CM)
           / ENCODER_COUNTS_PER_OUTPUT_REV;
}
int32_t Encoder_GetLeftCount(void)
{
    return (int32_t)leftEncoder.getCount();
}

int32_t Encoder_GetRightCount(void)
{
    return (int32_t)rightEncoder.getCount();
}

void Encoder_Reset(void)
{
    leftEncoder.clearCount();
    rightEncoder.clearCount();
}