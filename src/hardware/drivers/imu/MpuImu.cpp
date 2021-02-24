#include "MpuImu.h"

namespace IMUDrivers {
    namespace { // private
        constexpr uint32_t GYROSCOPE_SAMPLE_RATE_SPI = 8000;
        constexpr uint32_t GYROSCOPE_SAMPLE_RATE_I2C = 1000; // TODO we may as well enable DLPF when using 1kHz anyway
        constexpr uint32_t ACCELEROMETER_SAMPLE_RATE = 1000;

        constexpr double GYROSCOPE_DPS_PER_LSB = 2000.0 / 32768.0; // +-2000 degrees per second sensor range, -32768 to +32767 ADC range
        constexpr double ACCELEROMETER_Gs_PER_LSB = 16.0 / 32768.0; // +-16G sensor range, -32768 to +32767 ADC range

        constexpr uint8_t GYROSCOPE_CONFIG_VALUE = 0b00011000; // Self Tests disabled, +-2000 degrees per second full scale range
        constexpr uint8_t ACCELEROMETER_CONFIG_VALUE = 0b00011000; // Self Tests disabled, +-16g full scale range
        constexpr uint8_t POWER_MANAGEMENT_1_VALUE = 0b00000011; // Sleep disabled (default enabled), Clock source = Z axis imu (improves stability)
        constexpr uint8_t SIGNAL_PATH_RESET_RESET = 0b00000111; // Resets the device's signal paths
        constexpr uint8_t POWER_MANAGEMENT_1_RESET = 0b10000000; // Resets the device
        constexpr uint8_t USER_CONTROL_I2C_DISABLED_VALUE = 0b00010000; // Disables I2C interface (for when using SPI)
        constexpr uint8_t I2C_SAMPLE_RATE_DIVIDER = 8; // 8kHz -> 1kHz as I2C can't handle the bandwidth

        const SPISettings defaultSettings = SPISettings(1000000, MSBFIRST, SPI_MODE0);
        // When reading sensor register data, SPI can be clocked at 20MHz
        const SPISettings fastSettings = SPISettings(20000000, MSBFIRST, SPI_MODE0);

        // Register addresses
        constexpr uint8_t sampleRateDividerAddress = 25;
        constexpr uint8_t gyroscopeConfigAddress = 27;
        constexpr uint8_t accelerometerConfigAddress = 28;
        constexpr uint8_t accelerometerXOutputHighAddress = 59;
        constexpr uint8_t gyroscopeXOutputHighAddress = 67;
        constexpr uint8_t signalPathResetAddress = 104;
        constexpr uint8_t userControlAddress = 106;
        constexpr uint8_t powerManagement1Address = 107;

        Hardware::ThreeAxisData rawDataToThreeAxisData(const uint8_t data[6], const double unitsPerLSB) {
            return Hardware::ThreeAxisData {
                .x = static_cast<double>(static_cast<int16_t>((data[0] << 8) | data[1])) * unitsPerLSB,
                .y = static_cast<double>(static_cast<int16_t>((data[2] << 8) | data[3])) * unitsPerLSB,
                .z = static_cast<double>(static_cast<int16_t>((data[4] << 8) | data[5])) * unitsPerLSB,
            };
        }
    }

    MpuImu::MpuImu(TwoWire& i2c, uint8_t address) : Hardware::Gyroscope(GYROSCOPE_SAMPLE_RATE_I2C), Hardware::Accelerometer(ACCELEROMETER_SAMPLE_RATE), device(new BusIO::I2CDevice(&i2c, address)) {
        this->initialize(true);
    }

    MpuImu::MpuImu(SPIClass& spi, uint32_t csPin) : Hardware::Gyroscope(GYROSCOPE_SAMPLE_RATE_SPI), Hardware::Accelerometer(ACCELEROMETER_SAMPLE_RATE) {
        auto spiDevice = new BusIO::SPIDevice(&spi, defaultSettings, csPin);
        this->device = spiDevice;
        this->initialize(false);
        // Now that setup is complete we can switch to faster SPI.
        spiDevice->setSettings(fastSettings);
    }

    void MpuImu::initialize(bool usingI2C) {
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
        if (usingI2C) {
            this->device->writeRegister(sampleRateDividerAddress, I2C_SAMPLE_RATE_DIVIDER);
        } else {
            this->device->writeRegister(userControlAddress, USER_CONTROL_I2C_DISABLED_VALUE);
        }
    }

    Hardware::ThreeAxisData MpuImu::getRotationData() {
        uint8_t buffer[6];
        this->device->burstRead(gyroscopeXOutputHighAddress, buffer, 6);
        // TODO calibration
        return rawDataToThreeAxisData(buffer, GYROSCOPE_DPS_PER_LSB);
    }

    Hardware::ThreeAxisData MpuImu::getAccelerationData() {
        uint8_t buffer[6];
        this->device->burstRead(accelerometerXOutputHighAddress, buffer, 6);
        // TODO calibration
        return rawDataToThreeAxisData(buffer, ACCELEROMETER_Gs_PER_LSB);
    }
}
