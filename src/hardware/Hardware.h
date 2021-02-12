#pragma once

#include <Arduino.h>

#include <SPI.h>

namespace Hardware {
    /**
     * Initialize all hardware
     */
    void initialize();

    /**
     * The configuration UART interface, if it exists (null if it doesn't).
     * Typically USB.
     */
    extern const Stream* configurationUart;

    /**
     * Number of available UARTs
     */
    extern const size_t uartCount;

    /**
     * All available UARTs except for the configuration UART
     * Contains uartCount elements, all accesses must have their indexes checked!
     */
    extern const HardwareSerial UARTs[];

    /**
     * Number of available SPI buses
     */
    extern const size_t spiCount;

    /**
     * All available SPIs
     * Contains spiCount elements, all accesses must have their indexes checked!
     */
    extern const SPIClass SPIs[];
}
