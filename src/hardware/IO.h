#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <etl/vector.h>
#include <etl/map.h>
#include <etl/string.h>
#include "hardware/target/target.h"
#include "config/Config.h"
#include "AbstractSerialPort.h"
#include "Pin.h"

/**
 * IO is hardware that is part of the microcontroller.
 * This abstraction is needed to target multiple microcontrollers.
 */
namespace IO {
    constexpr size_t maxNumberOfUARTs = sizeof(IOConfig::uartConfigs) / sizeof(UartConfig) + sizeof(IOConfig::softwareUartConfigs) / sizeof(UartConfig) + 1;
    constexpr size_t maxNumberOfI2Cs = sizeof(IOConfig::i2cConfigs) / sizeof(I2CConfig);
    constexpr size_t maxNumberOfSPIs = sizeof(IOConfig::spiConfigs) / sizeof(SPIConfig);

    static constexpr size_t UartNameSize = sizeof(UartConfig::name) / sizeof(char);
    using UartName = etl::string<UartNameSize>;

    /**
     * All available UARTs except for the config UART.
     * Initialization is affected by hardware config.
     */
    extern etl::map<UartName, SerialPort*, maxNumberOfUARTs> UARTs;

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

    /**
     * Initialize all hardware and the watchdog timer
     */
    void initialize();

    /**
     * Convert pin name to number
     * @param pinName The pin name
     * @return The pin number, or null if one with that name was not found
     */
    const Pin* findPin(char* pinName);

    /**
     * Attempts to exclusively take a UART. Doing so will make the UART unavailable for use by other drivers.
     * @param uartIndex The UART index
     * @return The UART or nullptr if it does not exist or was taken.
     */
    extern SerialPort* takeUart(const UartName& uartName);
}
