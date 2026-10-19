
#include "Config.h"
#include "RobotState.h"
#include <UltrasonicSensorLibrary.h>
#include "MotorControl.h"
#include "Encoder.h"
#include "MotionControl.h"
#include "ObstacleAvoidance.h"
#include "CoveragePlanner.h"
#include "RobotController.h"

Ultrasonic ultrasonic(
    ULTRASONIC_TRIG_PIN,
    ULTRASONIC_ECHO_PIN
);

RobotState robot;

static unsigned long startupTime = 0;
static bool coverageStartRequested = false;

void setup()
{
    Serial.begin(SERIAL_BAUD_RATE);

    robot.mode = MODE_IDLE;
    robot.status = STATUS_IDLE;

    robot.speed_mps = 0.0f;
    robot.distance_m = 0.0f;
    robot.x_m = 0.0f;
    robot.y_m = 0.0f;
    robot.heading_deg = 0.0f;
    robot.coverage_percent = 0.0f;

    robot.obstacle_distance_cm = -1.0f;
    robot.obstacle_detected = false;

    Motor_Init();
    Motion_Init();
    Encoder_Init();

    ultrasonic.begin();

    ObstacleAvoidance_Init();
    RobotController_Init();

    startupTime = millis();

    Serial.println("ESP32 Rover Firmware Started");
    Serial.println("Coverage starts in 3 seconds");
}

void loop()
{
    RobotController_Update();

    if (!coverageStartRequested &&
        millis() - startupTime >= 3000UL)
    {
        coverageStartRequested = true;
        RobotController_StartCoverage();

        Serial.println("Coverage start requested");
    }

    delay(20);
}
