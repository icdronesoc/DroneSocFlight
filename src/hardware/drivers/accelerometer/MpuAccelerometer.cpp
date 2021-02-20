#include "MpuAccelerometer.h"

namespace AccelerometerDrivers {
    namespace { // private
        constexpr uint32_t SAMPLE_RATE = 1000;
        constexpr uint32_t ACCELEROMETER_CONFIG = 0b00011000; // Self Tests disabled, +-16g full scale range

        const SPISettings defaultSettings = SPISettings(1000000, MSBFIRST, SPI_MODE0);
        // When reading sensor register data, SPI can be clocked at 20MHz
        const SPISettings fastSettings = SPISettings(20000000, MSBFIRST, SPI_MODE0);

        // Register addresses
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
        constexpr uint8_t signalPathResetAddress = 104;
        constexpr uint8_t userControlAddress = 106;
        constexpr uint8_t powerManagement1Address = 107;
        constexpr uint8_t powerManagement2Address = 108;
        constexpr uint8_t fifoCountHighAddress = 114;
        constexpr uint8_t fifoCountLowAddress = 115;
        constexpr uint8_t fifoRWAddress = 116;
        constexpr uint8_t whoAmIAddress = 117;
    }

    MpuAccelerometer::MpuAccelerometer(TwoWire* i2c, uint8_t address) : Hardware::Accelerometer(SAMPLE_RATE), device(new BusIO::I2CDevice(i2c, address)) {
        this->initialize();
    }

    MpuAccelerometer::MpuAccelerometer(SPIClass* spi, uint32_t csPin) : Hardware::Accelerometer(SAMPLE_RATE) {
        auto spiDevice = new BusIO::SPIDevice(spi, defaultSettings, csPin);
        this->device = spiDevice;
        this->initialize();
        // Now that setup is complete we can switch to faster SPI.
        spiDevice->setSettings(fastSettings);
    }

    void MpuAccelerometer::initialize() {
        // Default sample rate divider: Full Sample rate
        // Default device config: External Sync disabled, Digital Low Pass Filter disabled
        this->device->writeRegister(accelerometerConfigAddress, ACCELEROMETER_CONFIG);
        // Default FIFO enable: All FIFOs disabled
        // TODO finish setup
    }

    Hardware::ThreeAxisData MpuAccelerometer::getAccelerationData() {
        // TODO
    }
}
