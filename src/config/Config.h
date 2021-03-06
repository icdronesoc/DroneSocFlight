#pragma once

#include <Arduino.h>
#include "Config.pb.h"

namespace Config {
    /**
     * Config holds all software configuration.
     * This is represented by the data structures generated from Config.proto.
     */
    extern Configuration config;

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
