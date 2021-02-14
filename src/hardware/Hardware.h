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
     * Setup hardware as described in the board configuration
     * @param hardwareConfiguration The hardware configuration, to initialize the correct drivers with the correct parameters
     * @param softwareConfiguration The software configuration, primarily to be passed to the hardware drivers.
     */
    void initialize();

    /**
     * The accelerometer, or null if none is configured
     */
    extern Accelerometer* accelerometer;

    /**
     * The gyroscope, or null if none is configured
     */
    extern Gyroscope* gyroscope;

    constexpr size_t maxMotorCount = sizeof(HardwareConfiguration::motors) / sizeof(MotorConfig);
    extern etl::vector<Motor*, maxMotorCount> motors;

    constexpr size_t maxServoCount = sizeof(HardwareConfiguration::servos) / sizeof(ServoConfig);
    extern etl::vector<Servo*, maxServoCount> servos;
}
