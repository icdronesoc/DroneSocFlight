#include "MpuImu.h"

namespace IMUDrivers {
    namespace { // private
        constexpr uint32_t ACCELEROMETER_SAMPLE_RATE = 1000;
        constexpr uint32_t GYROSCOPE_SAMPLE_RATE = 8000;
        constexpr uint8_t GYROSCOPE_CONFIG_VALUE = 0b00011000; // Self Tests disabled, +-2000 degrees per second full scale range
        constexpr uint8_t ACCELEROMETER_CONFIG_VALUE = 0b00011000; // Self Tests disabled, +-16g full scale range
        constexpr uint8_t POWER_MANAGEMENT_1_VALUE = 0b00000011; // Sleep disabled (default enabled), Clock source = Z axis imu (improves stability)
        constexpr uint8_t SIGNAL_PATH_RESET_RESET = 0b00000111; // Resets the device's signal paths
        constexpr uint8_t POWER_MANAGEMENT_1_RESET = 0b10000000; // Resets the device
        constexpr uint8_t USER_CONTROL_I2C_DISABLED_VALUE = 0b00010000; // Disables I2C interface (for when using SPI)
        constexpr uint8_t INTERRUPT_PIN_CONFIG_VALUE = 0b00010000; // Only clear interrupt flags when reading interrupt status register
        constexpr uint8_t INTERRUPT_PIN_ENABLED_VALUE = 0b00000001; // For when interrupts are enabled
        constexpr uint8_t INTERRUPT_PIN_DISABLED_VALUE = 0b00000000; // For when interrupts are enabled

        const SPISettings defaultSettings = SPISettings(1000000, MSBFIRST, SPI_MODE0);
        // When reading sensor register data, SPI can be clocked at 20MHz
        const SPISettings fastSettings = SPISettings(20000000, MSBFIRST, SPI_MODE0);

        // Register addresses
        constexpr uint8_t gyroscopeConfigAddress = 27;
        constexpr uint8_t accelerometerConfigAddress = 28;
        constexpr uint8_t i2cMasterControlAddress = 36;
        constexpr uint8_t interruptPinConfigAddress = 55;
        constexpr uint8_t interruptPinEnableAddress = 56;
        constexpr uint8_t interruptPinStatusAddress = 58;
        constexpr uint8_t accelerometerXOutputHighAddress = 59;
        constexpr uint8_t accelerometerXOutputLowAddress = 60;
        constexpr uint8_t accelerometerYOutputHighAddress = 61;
        constexpr uint8_t accelerometerYOutputLowAddress = 62;
        constexpr uint8_t accelerometerZOutputHighAddress = 63;
        constexpr uint8_t accelerometerZOutputLowAddress = 64;
        constexpr uint8_t temperatureOutputHighAddress = 65;
        constexpr uint8_t temperatureOutputLowAddress = 66;
        constexpr uint8_t gyroscopeXOutputHighAddress = 67;
        constexpr uint8_t gyroscopeXOutputLowAddress = 68;
        constexpr uint8_t gyroscopeYOutputHighAddress = 69;
        constexpr uint8_t gyroscopeYOutputLowAddress = 70;
        constexpr uint8_t gyroscopeZOutputHighAddress = 71;
        constexpr uint8_t gyroscopeZOutputLowAddress = 72;
        constexpr uint8_t signalPathResetAddress = 104;
        constexpr uint8_t userControlAddress = 106;
        constexpr uint8_t powerManagement1Address = 107;
        constexpr uint8_t powerManagement2Address = 108;
        constexpr uint8_t fifoCountHighAddress = 114;
        constexpr uint8_t fifoCountLowAddress = 115;
        constexpr uint8_t fifoRWAddress = 116;
        constexpr uint8_t whoAmIAddress = 117;
    }

    MpuImu::MpuImu(TwoWire* i2c, uint8_t address, IO::Pin* interruptPin) : Hardware::Gyroscope(GYROSCOPE_SAMPLE_RATE), Hardware::Accelerometer(ACCELEROMETER_SAMPLE_RATE), interruptPin(interruptPin), device(new BusIO::I2CDevice(i2c, address)) {
        this->initialize(false);
    }

    MpuImu::MpuImu(SPIClass* spi, uint32_t csPin, IO::Pin* interruptPin) : Hardware::Gyroscope(GYROSCOPE_SAMPLE_RATE), Hardware::Accelerometer(ACCELEROMETER_SAMPLE_RATE), interruptPin(interruptPin) {
        auto spiDevice = new BusIO::SPIDevice(spi, defaultSettings, csPin);
        this->device = spiDevice;
        this->initialize(true);
        // Now that setup is complete we can switch to faster SPI.
        spiDevice->setSettings(fastSettings);
    }

    void MpuImu::initialize(bool disableDeviceI2C) {
        // Reset device
        this->device->writeRegister(powerManagement1Address, POWER_MANAGEMENT_1_RESET);
        delay(100);
        this->device->writeRegister(signalPathResetAddress, SIGNAL_PATH_RESET_RESET);
        delay(100);

        // Configure device
        this->device->writeRegister(powerManagement1Address, POWER_MANAGEMENT_1_VALUE);
        delayMicroseconds(15);
        this->device->writeRegister(accelerometerConfigAddress, ACCELEROMETER_CONFIG_VALUE);
        delayMicroseconds(15);
        this->device->writeRegister(gyroscopeConfigAddress, GYROSCOPE_CONFIG_VALUE);
        delayMicroseconds(15);
        if (disableDeviceI2C) {
            this->device->writeRegister(userControlAddress, USER_CONTROL_I2C_DISABLED_VALUE);
            delayMicroseconds(15);
        }
        this->device->writeRegister(interruptPinConfigAddress, INTERRUPT_PIN_CONFIG_VALUE);
        delayMicroseconds(15);
        this->device->writeRegister(interruptPinEnableAddress, this->interruptPin == nullptr ? INTERRUPT_PIN_DISABLED_VALUE : INTERRUPT_PIN_ENABLED_VALUE);
    }

    Hardware::ThreeAxisData MpuImu::getRotationData() {
        // TODO
    }

    Hardware::ThreeAxisData MpuImu::getAccelerationData() {
        // TODO
    }
}
