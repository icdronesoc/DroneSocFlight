#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <etl/vector.h>
#include "config/Config.h"
#include "AbstractSerialPort.h"

/**
 * IO is hardware that is part of the microcontroller.
 * This abstraction is needed to target multiple microcontrollers.
 */
namespace IO {
    /**
     * Initialize all hardware and the watchdog timer
     */
    void initialize();

    /**
     * Reset the watchdog timer
     */
    void resetWatchdogTimer();

    constexpr size_t maxPinNameLength = sizeof(Pin::pinName) / sizeof(char);
    constexpr size_t maxNumberOfUARTs = sizeof(IOConfig::uartConfigs) / sizeof(UartConfig) + sizeof(IOConfig::softwareUartConfigs) / sizeof(UartConfig); // TODO when config UART gets moved here, always add 1
    constexpr size_t maxNumberOfI2Cs = sizeof(IOConfig::i2cConfigs) / sizeof(I2CConfig);
    constexpr size_t maxNumberOfSPIs = sizeof(IOConfig::spiConfigs) / sizeof(SPIConfig);

    struct Pin {
        char name[maxPinNameLength];
        uint32_t number;
    };

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
     * @param pinName The pin name
     * @return The pin number, or null if one with that name was not found
     */
    Pin* findPin(char* pinName);

    /**
     * All available UARTs except for the config UART.
     * Initialization is affected by hardware config.
     * TODO give UARTs names
     */
    extern etl::vector<SerialPort*, maxNumberOfUARTs> UARTs;

    /**
     * Attempts to exclusively take a UART. Doing so will make the UART unavailable for use by other drivers.
     * @param uartIndex The UART index
     * @return The UART or nullptr if it does not exist or was taken.
     */
    extern SerialPort* takeUart(size_t uartIndex);

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
