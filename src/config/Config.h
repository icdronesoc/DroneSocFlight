#pragma once

#include <Arduino.h>

namespace Config {
    /**
     * Hardware Configuration holds information about how the FC board and attached peripherals are configured.
     */
    typedef struct {
    } HardwareConfiguration;

    /**
     * Software Configuration holds information about how the FC software is configured.
     * This should be completely board-agnostic and portable between different boards.
     */
    typedef struct {

    } SoftwareConfiguration;
}
