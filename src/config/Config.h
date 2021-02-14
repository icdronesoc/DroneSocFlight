#pragma once

#include <Arduino.h>
#include "Config.pb.h"

namespace Config {
    /**
     * Hardware Configuration holds information about how the FC board and attached peripherals are configured.
     * This is just an alias for the Protobuf type.
     */
    typedef HardwareConfiguration HardwareConfiguration;

    /**
     * Software Configuration holds information about how the FC software is configured.
     * This should be completely board-agnostic and portable between different boards.
     */
    typedef struct {

    } SoftwareConfiguration;
}
