#include "MpuGyroscope.h"

namespace GyroscopeDrivers {
    namespace { // private
        constexpr uint32_t SAMPLE_RATE = 8000;
        constexpr uint32_t GYROSCOPE_CONFIG = 0b00011000; // Self Tests disabled, +-2000 degrees per second full scale range

        const SPISettings defaultSettings = SPISettings(1000000, MSBFIRST, SPI_MODE0);
        // When reading sensor register data, SPI can be clocked at 20MHz
        const SPISettings fastSettings = SPISettings(20000000, MSBFIRST, SPI_MODE0);

        // Register addresses
        constexpr uint8_t gyroscopeConfigAddress = 27;
        constexpr uint8_t i2cMasterControlAddress = 36;
        constexpr uint8_t interruptPinConfigAddress = 55;
        constexpr uint8_t interruptPinEnableAddress = 56;
        constexpr uint8_t interruptPinStatusAddress = 58;
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

    MpuGyroscope::MpuGyroscope(TwoWire* i2c, uint8_t address) : Hardware::Gyroscope(SAMPLE_RATE), device(new BusIO::I2CDevice(i2c, address)) {
        this->initialize();
    }

    MpuGyroscope::MpuGyroscope(SPIClass* spi, uint32_t csPin) : Hardware::Gyroscope(SAMPLE_RATE) {
        auto spiDevice = new BusIO::SPIDevice(spi, defaultSettings, csPin);
        this->device = spiDevice;
        this->initialize();
        // Now that setup is complete we can switch to faster SPI.
        spiDevice->setSettings(fastSettings);
    }

    void MpuGyroscope::initialize() {
        // Default sample rate divider: Full Sample rate
        // Default device config: External Sync disabled, Digital Low Pass Filter disabled
        this->device->writeRegister(gyroscopeConfigAddress, GYROSCOPE_CONFIG);
        // Default FIFO enable: All FIFOs disabled
        // TODO finish setup
    }

    Hardware::ThreeAxisData MpuGyroscope::getRotationData() {
        // TODO
    }
}
