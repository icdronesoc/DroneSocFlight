#pragma once

#include <Arduino.h>
#include "hardware/AbstractHardware.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_SPIDevice.h>
#include <Adafruit_I2CDevice.h>

namespace GyroscopeDrivers {
    /**
     * MPU6000 / MPU6050 gyroscope driver.
     *
     * Datasheets:
     * https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf
     * https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
     */
    class MpuGyroscope : public Hardware::Gyroscope {
    public:
        MpuGyroscope(TwoWire* i2c, uint8_t address);
        MpuGyroscope(SPIClass* spi, uint32_t csPin);

        void initialize() override;
        Hardware::ThreeAxisData getRotationData() override;

    private:
        // Device registers
        Adafruit_BusIO_Register* selfTestX;
        Adafruit_BusIO_Register* selfTestY;
        Adafruit_BusIO_Register* selfTestZ;
        Adafruit_BusIO_Register* selfTestA;
        Adafruit_BusIO_Register* sampleRateDivider;
        Adafruit_BusIO_Register* config;
        Adafruit_BusIO_Register* gyroConfig;
        Adafruit_BusIO_Register* accelerometerConfig;
        Adafruit_BusIO_Register* fifoEnabled;
        Adafruit_BusIO_Register* i2cMasterControl;
        Adafruit_BusIO_Register* interruptPinConfig;
        Adafruit_BusIO_Register* interruptPinEnable;
        Adafruit_BusIO_Register* interruptPinStatus;
        Adafruit_BusIO_Register* accelerometerXOutputHigh;
        Adafruit_BusIO_Register* accelerometerXOutputLow;
        Adafruit_BusIO_Register* accelerometerYOutputHigh;
        Adafruit_BusIO_Register* accelerometerYOutputLow;
        Adafruit_BusIO_Register* accelerometerZOutputHigh;
        Adafruit_BusIO_Register* accelerometerZOutputLow;
        Adafruit_BusIO_Register* temperatureOutputHigh;
        Adafruit_BusIO_Register* temperatureOutputLow;
        Adafruit_BusIO_Register* gyroscopeXOutputHigh;
        Adafruit_BusIO_Register* gyroscopeXOutputLow;
        Adafruit_BusIO_Register* gyroscopeYOutputHigh;
        Adafruit_BusIO_Register* gyroscopeYOutputLow;
        Adafruit_BusIO_Register* gyroscopeZOutputHigh;
        Adafruit_BusIO_Register* gyroscopeZOutputLow;
        Adafruit_BusIO_Register* signalPathReset;
        Adafruit_BusIO_Register* userControl;
        Adafruit_BusIO_Register* powerManagement1;
        Adafruit_BusIO_Register* powerManagement2;
        Adafruit_BusIO_Register* fifoCountHigh;
        Adafruit_BusIO_Register* fifoCountLow;
        Adafruit_BusIO_Register* fifoRW;
        Adafruit_BusIO_Register* whoAmI;
    };
}
