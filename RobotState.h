
#ifndef ROBOT_STATE_H
#define ROBOT_STATE_H

#include <Arduino.h>

enum RobotMode {
    MODE_IDLE,
    MODE_COVERAGE,
    MODE_MANUAL
};

enum RobotStatus {
    STATUS_IDLE,
    STATUS_RUNNING,
    STATUS_PAUSED,
    STATUS_OBSTACLE,
    STATUS_ERROR
};

struct RobotState {
    RobotMode mode;
    RobotStatus status;

    float speed_mps;
    float distance_m;
    float x_m;
    float y_m;
    float heading_deg;
    float coverage_percent;

    float obstacle_distance_cm;
    bool obstacle_detected;
};

#endif
