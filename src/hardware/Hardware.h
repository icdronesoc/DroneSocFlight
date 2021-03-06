#pragma once

#include <Arduino.h>
#include "AbstractHardware.h"
#include "config/Config.h"
#include "etl/vector.h"

/**
 * Hardware is hardware that is on, or connected to, the FC board.
 */
namespace Hardware {
    /**
     * Setup hardware as described in the board config. This expects that all IO has been initialized.
     */
    void initialize();

    /**
     * The accelerometer, or null if none is configured
     */
    extern Accelerometer* accelerometer;

    /**
     * The imu, or null if none is configured
     */
    extern Gyroscope* gyroscope;

    constexpr size_t maxMotorCount = sizeof(Configuration::motors) / sizeof(MotorConfig);
    extern etl::vector<Motor*, maxMotorCount> motors;

    constexpr size_t maxServoCount = sizeof(Configuration::servos) / sizeof(ServoConfig);
    extern etl::vector<Servo*, maxServoCount> servos;
}
