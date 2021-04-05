#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

namespace BusIO {
    class RegisterBasedDevice {
    public:
        virtual uint8_t readRegister(uint8_t address) = 0;
        virtual void burstRead(uint8_t address, uint8_t* buffer, size_t length) = 0;
        virtual void writeRegister(uint8_t address, uint8_t data) = 0;
        virtual void burstWrite(uint8_t address, uint8_t* buffer, size_t length) = 0;
        virtual void strobeRegister(uint8_t address) = 0;
    };

    class SPIDevice final : public RegisterBasedDevice {
    public:
        SPIDevice(SPIClass* spi, SPISettings settings, uint32_t csPin);
        void setSettings(SPISettings newSettings);
        uint8_t readRegister(uint8_t address) override;
        void burstRead(uint8_t address, uint8_t* buffer, size_t length) override;
        void writeRegister(uint8_t address, uint8_t data) override;
        void burstWrite(uint8_t address, uint8_t* buffer, size_t length) override;
        void strobeRegister(uint8_t address) override;

    private:
        SPIClass* spi;
        SPISettings settings;
        uint32_t csPin;
    };

    class I2CDevice final : public RegisterBasedDevice {
    public:
        I2CDevice(TwoWire* i2c, uint8_t deviceAddress);
        uint8_t readRegister(uint8_t address) override;
        void burstRead(uint8_t address, uint8_t* buffer, size_t length) override;
        void writeRegister(uint8_t address, uint8_t data) override;
        void burstWrite(uint8_t address, uint8_t* buffer, size_t length) override;
        void strobeRegister(uint8_t address) override;

    private:
        TwoWire* i2c;
        uint8_t deviceAddress;
    };
}
