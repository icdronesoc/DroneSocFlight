#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

namespace Hardware {
    /**
     * Initialize all hardware
     */
    void initialize();

    /**
     * The configuration UART interface, if it exists (null if it doesn't).
     * Typically USB.
     */
    extern Stream* configurationUart;

    /**
     * Number of available UARTs
     */
    extern size_t uartCount;

    /**
     * All available UARTs except for the configuration UART
     * Contains uartCount elements, all accesses must have their indexes checked!
     */
    extern HardwareSerial UARTs[];

    /**
     * Number of available SPI buses
     */
    extern size_t spiCount;

    /**
     * All available SPIs
     * Contains spiCount elements, all accesses must have their indexes checked!
     */
    extern SPIClass SPIs[];

    /**
     * Number of available I2C buses
     */
    extern size_t i2cCount;

    /**
     * All available I2Cs
     * Contains i2cCount elements, all accesses must have their indexes checked!
     */
    extern TwoWire I2Cs[];
}
