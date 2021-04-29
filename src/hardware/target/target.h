#pragma once

#include <Arduino.h>
#include "hardware/Pin.h"

namespace Target {

    /**
     * Resets the WDT
     */
    void resetWatchdogTimer();

    /**
     * A list of all pins the MCU has that the user should have access to
     */
    extern const IO::Pin pins[];

    extern const size_t pinCount;

    /**
     * Sets up the MCU's hardware
     */
    void setupMcuHardware();

    /**
     * Loads data from non-volatile storage. TODO move to dedicated storage module
     * @param buffer The buffer to store the data into.
     * @param maxSize The maximum number of bytes to read (the size of the buffer)
     * @return The number of bytes read, or 0 if an error occurred (eg. the buffer was not big enough)
     */
    size_t loadData(byte *buffer, size_t maxSize);

    /**
     * Stores data in non-volatile storage. TODO move to dedicated storage module
     * Overwrites any existing data.
     * @param data The data to write
     * @param length The length of the data to write
     * @return true if the store was successful, false if it was not (eg. the non-volatile storage is not big enough)
     */
    bool storeData(byte *data, size_t length);
}

// Include the relevant target file **after** the definitions. Definition after declaration is not allowed in C/C++.
//#if defined(PLATFORM_STM32)
//#include "hardware/target/stm32/target_stm32.h"
//#elif defined(PLATFORM_ESP32)
//#include "hardware/target/esp32/target_esp32.h"
//#else
//#error "No platform defined."
//#endif
