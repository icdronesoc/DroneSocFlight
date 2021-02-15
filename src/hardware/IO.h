#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <etl/vector.h>
#include "config/Config.h"
#include "AbstractIO.h"

/**
 * IO is hardware that is part of the microcontroller.
 * This abstraction is needed to target multiple microcontrollers.
 */
namespace IO {
    /**
     * Initialize all hardware
     */
    void initialize();

    constexpr size_t maxPinNameLength = sizeof(Pin::pinName) / sizeof(char);
    constexpr size_t maxNumberOfUARTs = sizeof(IOConfig::uartConfigs) / sizeof(UartConfig) + sizeof(IOConfig::softwareUartConfigs) / sizeof(UartConfig); // TODO when config UART gets moved here, always add 1
    constexpr size_t maxNumberOfI2Cs = sizeof(IOConfig::i2cConfigs) / sizeof(I2cConfig);
    constexpr size_t maxNumberOfSPIs = sizeof(IOConfig::spiConfigs) / sizeof(SpiConfig);

    typedef struct {
        char name[maxPinNameLength];
        uint32_t number;
    } Pin;

    /**
     * All available IO pins.
     */
    extern Pin pins[];

    /**
     * Number of IO pins
     */
    extern const size_t pinCount;

    /**
     * Convert pin name to number
     * TODO handle error properly
     * @param pinName The pin name
     * @return The pin number, or 0 if error TODO
     */
    uint32_t pinNameToNumber(char* pinName);

    /**
     * All available UARTs except for the config UART.
     * Initialization is affected by hardware config.
     * TODO give UARTs names
     */
    extern etl::vector<SerialPort*, maxNumberOfUARTs> UARTs;

    /**
     * All available SPIs.
     * Initialization is affected by hardware config.
     */
    extern etl::vector<SPIClass*, maxNumberOfSPIs> SPIs;

    /**
     * All available I2Cs.
     * Initialization is affected by hardware config.
     */
    extern etl::vector<TwoWire*, maxNumberOfI2Cs> I2Cs;
}
