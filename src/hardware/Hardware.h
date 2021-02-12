#pragma once

#include <Arduino.h>

namespace HardwareConfig {
    /**
     * Initialize all hardware
     */
    void initialize();

    /**
     * The configuration UART interface, if it exists (null if it doesn't).
     * Typically USB.
     */
    const extern Stream* configurationUart;

    /**
     * Number of available UARTs
     */
    const extern size_t uartCount;

    /**
     * Safely get a UART
     * @param uart The UART number, zero-indexed
     * @return The UART if it exists, or null if it does not
     */
    const HardwareSerial* getUart(size_t uart);
}
