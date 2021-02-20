#include "MpuAccelerometer.h"

namespace AccelerometerDrivers {
    MpuAccelerometer::MpuAccelerometer(TwoWire* i2c, uint8_t address) {
        auto device = new Adafruit_I2CDevice(address, i2c);

        this->selfTestX = new Adafruit_BusIO_Register(device, 13, ADDRBIT8_HIGH_TOREAD);
        this->selfTestY = new Adafruit_BusIO_Register(device, 14, ADDRBIT8_HIGH_TOREAD);
        this->selfTestZ = new Adafruit_BusIO_Register(device, 15, ADDRBIT8_HIGH_TOREAD);
        this->selfTestA = new Adafruit_BusIO_Register(device, 16, ADDRBIT8_HIGH_TOREAD);
        this->sampleRateDivider = new Adafruit_BusIO_Register(device, 25, ADDRBIT8_HIGH_TOREAD);
        this->config = new Adafruit_BusIO_Register(device, 26, ADDRBIT8_HIGH_TOREAD);
        this->gyroConfig = new Adafruit_BusIO_Register(device, 27, ADDRBIT8_HIGH_TOREAD);
        this->accelerometerConfig = new Adafruit_BusIO_Register(device, 28, ADDRBIT8_HIGH_TOREAD);
        this->fifoEnabled = new Adafruit_BusIO_Register(device, 35, ADDRBIT8_HIGH_TOREAD);
        this->i2cMasterControl = new Adafruit_BusIO_Register(device, 36, ADDRBIT8_HIGH_TOREAD);
        this->interruptPinConfig = new Adafruit_BusIO_Register(device, 55, ADDRBIT8_HIGH_TOREAD);
        this->interruptPinEnable = new Adafruit_BusIO_Register(device, 56, ADDRBIT8_HIGH_TOREAD);
        this->interruptPinStatus = new Adafruit_BusIO_Register(device, 58, ADDRBIT8_HIGH_TOREAD);
        this->accelerometerXOutputHigh = new Adafruit_BusIO_Register(device, 59, ADDRBIT8_HIGH_TOREAD);
        this->accelerometerXOutputLow = new Adafruit_BusIO_Register(device, 60, ADDRBIT8_HIGH_TOREAD);
        this->accelerometerYOutputHigh = new Adafruit_BusIO_Register(device, 61, ADDRBIT8_HIGH_TOREAD);
        this->accelerometerYOutputLow = new Adafruit_BusIO_Register(device, 62, ADDRBIT8_HIGH_TOREAD);
        this->accelerometerZOutputHigh = new Adafruit_BusIO_Register(device, 63, ADDRBIT8_HIGH_TOREAD);
        this->accelerometerZOutputLow = new Adafruit_BusIO_Register(device, 64, ADDRBIT8_HIGH_TOREAD);
        this->temperatureOutputHigh = new Adafruit_BusIO_Register(device, 65, ADDRBIT8_HIGH_TOREAD);
        this->temperatureOutputLow = new Adafruit_BusIO_Register(device, 66, ADDRBIT8_HIGH_TOREAD);
        this->gyroscopeXOutputHigh = new Adafruit_BusIO_Register(device, 67, ADDRBIT8_HIGH_TOREAD);
        this->gyroscopeXOutputLow = new Adafruit_BusIO_Register(device, 68, ADDRBIT8_HIGH_TOREAD);
        this->gyroscopeYOutputHigh = new Adafruit_BusIO_Register(device, 69, ADDRBIT8_HIGH_TOREAD);
        this->gyroscopeYOutputLow = new Adafruit_BusIO_Register(device, 70, ADDRBIT8_HIGH_TOREAD);
        this->gyroscopeZOutputHigh = new Adafruit_BusIO_Register(device, 71, ADDRBIT8_HIGH_TOREAD);
        this->gyroscopeZOutputLow = new Adafruit_BusIO_Register(device, 72, ADDRBIT8_HIGH_TOREAD);
        this->signalPathReset = new Adafruit_BusIO_Register(device, 104, ADDRBIT8_HIGH_TOREAD);
        this->userControl = new Adafruit_BusIO_Register(device, 106, ADDRBIT8_HIGH_TOREAD);
        this->powerManagement1 = new Adafruit_BusIO_Register(device, 107, ADDRBIT8_HIGH_TOREAD);
        this->powerManagement2 = new Adafruit_BusIO_Register(device, 108, ADDRBIT8_HIGH_TOREAD);
        this->fifoCountHigh = new Adafruit_BusIO_Register(device, 114, ADDRBIT8_HIGH_TOREAD);
        this->fifoCountLow = new Adafruit_BusIO_Register(device, 115, ADDRBIT8_HIGH_TOREAD);
        this->fifoRW = new Adafruit_BusIO_Register(device, 116, ADDRBIT8_HIGH_TOREAD);
        this->whoAmI = new Adafruit_BusIO_Register(device, 117, ADDRBIT8_HIGH_TOREAD);
    }

    MpuAccelerometer::MpuAccelerometer(SPIClass* spi, uint32_t csPin) {
        auto device = new Adafruit_SPIDevice(csPin, 100000, SPI_BITORDER_MSBFIRST, SPI_MODE0, spi);

        this->selfTestX = new Adafruit_BusIO_Register(device, 13, ADDRBIT8_HIGH_TOREAD);
        this->selfTestY = new Adafruit_BusIO_Register(device, 14, ADDRBIT8_HIGH_TOREAD);
        this->selfTestZ = new Adafruit_BusIO_Register(device, 15, ADDRBIT8_HIGH_TOREAD);
        this->selfTestA = new Adafruit_BusIO_Register(device, 16, ADDRBIT8_HIGH_TOREAD);
        this->sampleRateDivider = new Adafruit_BusIO_Register(device, 25, ADDRBIT8_HIGH_TOREAD);
        this->config = new Adafruit_BusIO_Register(device, 26, ADDRBIT8_HIGH_TOREAD);
        this->gyroConfig = new Adafruit_BusIO_Register(device, 27, ADDRBIT8_HIGH_TOREAD);
        this->accelerometerConfig = new Adafruit_BusIO_Register(device, 28, ADDRBIT8_HIGH_TOREAD);
        this->fifoEnabled = new Adafruit_BusIO_Register(device, 35, ADDRBIT8_HIGH_TOREAD);
        this->i2cMasterControl = new Adafruit_BusIO_Register(device, 36, ADDRBIT8_HIGH_TOREAD);
        this->interruptPinConfig = new Adafruit_BusIO_Register(device, 55, ADDRBIT8_HIGH_TOREAD);
        this->interruptPinEnable = new Adafruit_BusIO_Register(device, 56, ADDRBIT8_HIGH_TOREAD);
        this->interruptPinStatus = new Adafruit_BusIO_Register(device, 58, ADDRBIT8_HIGH_TOREAD);
        this->accelerometerXOutputHigh = new Adafruit_BusIO_Register(device, 59, ADDRBIT8_HIGH_TOREAD);
        this->accelerometerXOutputLow = new Adafruit_BusIO_Register(device, 60, ADDRBIT8_HIGH_TOREAD);
        this->accelerometerYOutputHigh = new Adafruit_BusIO_Register(device, 61, ADDRBIT8_HIGH_TOREAD);
        this->accelerometerYOutputLow = new Adafruit_BusIO_Register(device, 62, ADDRBIT8_HIGH_TOREAD);
        this->accelerometerZOutputHigh = new Adafruit_BusIO_Register(device, 63, ADDRBIT8_HIGH_TOREAD);
        this->accelerometerZOutputLow = new Adafruit_BusIO_Register(device, 64, ADDRBIT8_HIGH_TOREAD);
        this->temperatureOutputHigh = new Adafruit_BusIO_Register(device, 65, ADDRBIT8_HIGH_TOREAD);
        this->temperatureOutputLow = new Adafruit_BusIO_Register(device, 66, ADDRBIT8_HIGH_TOREAD);
        this->gyroscopeXOutputHigh = new Adafruit_BusIO_Register(device, 67, ADDRBIT8_HIGH_TOREAD);
        this->gyroscopeXOutputLow = new Adafruit_BusIO_Register(device, 68, ADDRBIT8_HIGH_TOREAD);
        this->gyroscopeYOutputHigh = new Adafruit_BusIO_Register(device, 69, ADDRBIT8_HIGH_TOREAD);
        this->gyroscopeYOutputLow = new Adafruit_BusIO_Register(device, 70, ADDRBIT8_HIGH_TOREAD);
        this->gyroscopeZOutputHigh = new Adafruit_BusIO_Register(device, 71, ADDRBIT8_HIGH_TOREAD);
        this->gyroscopeZOutputLow = new Adafruit_BusIO_Register(device, 72, ADDRBIT8_HIGH_TOREAD);
        this->signalPathReset = new Adafruit_BusIO_Register(device, 104, ADDRBIT8_HIGH_TOREAD);
        this->userControl = new Adafruit_BusIO_Register(device, 106, ADDRBIT8_HIGH_TOREAD);
        this->powerManagement1 = new Adafruit_BusIO_Register(device, 107, ADDRBIT8_HIGH_TOREAD);
        this->powerManagement2 = new Adafruit_BusIO_Register(device, 108, ADDRBIT8_HIGH_TOREAD);
        this->fifoCountHigh = new Adafruit_BusIO_Register(device, 114, ADDRBIT8_HIGH_TOREAD);
        this->fifoCountLow = new Adafruit_BusIO_Register(device, 115, ADDRBIT8_HIGH_TOREAD);
        this->fifoRW = new Adafruit_BusIO_Register(device, 116, ADDRBIT8_HIGH_TOREAD);
        this->whoAmI = new Adafruit_BusIO_Register(device, 117, ADDRBIT8_HIGH_TOREAD);
    }

    void MpuAccelerometer::initialize() {
        // TODO
    }

    Hardware::ThreeAxisData MpuAccelerometer::getAccelerationData() {
        // TODO
    }
}
