#pragma once

#include <Arduino.h>
#include "AbstractHardware.h"
#include "config/Config.h"

/**
 * Hardware is hardware that is on, or connected to, the FC board.
 */
namespace Hardware {
    /**
     * Setup hardware as described in the board configuration
     * @param hardwareConfiguration The hardware configuration, to initialize the correct drivers with the correct parameters
     * @param softwareConfiguration The software configuration, primarily to be passed to the hardware drivers.
     */
    void initialize(Config::HardwareConfiguration hardwareConfiguration, Config::SoftwareConfiguration softwareConfiguration);

    /**
     * The accelerometer, or null if none is configured
     */
    extern Accelerometer* accelerometer;

    /**
     * The gyroscope, or null if none is configured
     */
    extern Gyroscope* gyroscope;
}
