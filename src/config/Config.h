#pragma once

#include <Arduino.h>
#include "Config.pb.h"

namespace Config {
    /**
     * Hardware Configuration holds information about how the FC board and attached peripherals are configured.
     * This is determined by the protobuf type.
     */
    extern HardwareConfiguration hardwareConfiguration;

    /**
     * Software Configuration holds information about how the FC software is configured.
     * This should be completely board-agnostic and portable between different boards.
     */
     extern SoftwareConfiguration softwareConfiguration;

     /**
      * Loads hardware and software configuration from non-volatile storage (EEPROM or Flash).
      * Should be called immediately after boot, as until this is called the configurations will be their default value.
      */
     void loadConfiguration();

     /**
      * Stores configuration into non-volatile storage (EEPROM or Flash).
      * Once configuration has been changed, it should be stored and the FC should be rebooted,
      * in order to guarantee any changes are picked up.
      */
     void saveConfiguration();
}
