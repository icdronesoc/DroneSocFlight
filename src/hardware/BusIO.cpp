#include "BusIO.h"

namespace BusIO {
    namespace { // private
        constexpr uint8_t SPIReadBitMask = 0b10000000;
    }

    SPIDevice::SPIDevice(SPIClass* spi, SPISettings settings, uint32_t csPin) : spi(spi), settings(settings), csPin(csPin) {
        digitalWrite(csPin, HIGH); // TODO this order might cause issues but might solve them...
        pinMode(csPin, OUTPUT);
    }

    void SPIDevice::setSettings(SPISettings newSettings) {
        this->settings = newSettings;
    }

    uint8_t SPIDevice::readRegister(uint8_t address) {
        this->spi->beginTransaction(this->settings);
        digitalWrite(this->csPin, LOW);
        this->spi->transfer(SPIReadBitMask | address); // Set read bit
        auto result = this->spi->transfer(0);
        digitalWrite(this->csPin, HIGH);
        this->spi->endTransaction();
        return result;
    }

    void SPIDevice::burstRead(uint8_t address, uint8_t *buffer, size_t length) {
        this->spi->beginTransaction(this->settings);
        digitalWrite(this->csPin, LOW);
        this->spi->transfer(SPIReadBitMask | address); // Set read bit
        for (size_t i = 0; i < length; i++) {
            buffer[i] = this->spi->transfer(0);
        }
        digitalWrite(this->csPin, HIGH);
        this->spi->endTransaction();
    }

    void SPIDevice::writeRegister(uint8_t address, uint8_t data) {
        this->spi->beginTransaction(this->settings);
        digitalWrite(this->csPin, LOW);
        this->spi->transfer(address & ~SPIReadBitMask); // Clear read bit
        this->spi->transfer(data);
        digitalWrite(this->csPin, HIGH);
        this->spi->endTransaction();
    }

    I2CDevice::I2CDevice(TwoWire *i2c, uint8_t deviceAddress) : i2c(i2c), deviceAddress(deviceAddress) {}

    uint8_t I2CDevice::readRegister(uint8_t address) {
        this->i2c->beginTransmission(this->deviceAddress);
        this->i2c->write(address);
        this->i2c->endTransmission();
        this->i2c->requestFrom(address, static_cast<uint8_t>(1));
        while (!Wire.available()); // TODO timeout
        return this->i2c->read();
    }

    void I2CDevice::burstRead(uint8_t address, uint8_t *buffer, size_t length) {
        this->i2c->beginTransmission(this->deviceAddress);
        this->i2c->write(address);
        this->i2c->endTransmission();
        this->i2c->requestFrom(address, length);
        for (size_t i = 0; i < length; i++) {
            while (!Wire.available()); // TODO timeout
            buffer[i] = Wire.read();
        }
    }

    void I2CDevice::writeRegister(uint8_t address, uint8_t data) {
        this->i2c->beginTransmission(this->deviceAddress);
        this->i2c->write(address);
        this->i2c->write(data);
        this->i2c->endTransmission();
    }
}
