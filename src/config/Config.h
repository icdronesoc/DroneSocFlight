#pragma once

#include <Arduino.h>
#include "Config.pb.h"

namespace Config {
    /**
     * Hardware Config holds information about how the FC board and attached peripherals are configured.
     * This is determined by the protobuf type.
     */
    extern HardwareConfig hardwareConfig;

    /**
     * Software Config holds information about how the FC software is configured.
     * This should be completely board-agnostic and portable between different boards.
     */
     extern SoftwareConfig softwareConfig;

     /**
      * Loads hardware and software config from non-volatile storage (EEPROM or Flash).
      * Should be called immediately after boot, as until this is called the configs will be their default value.
      */
     void loadConfig();

     /**
      * Stores config into non-volatile storage (EEPROM or Flash).
      * Once config has been changed, it should be stored and the FC should be rebooted,
      * in order to guarantee any changes are picked up.
      */
     void saveConfig();
}
