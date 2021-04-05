#include "CC2500.h"
#include "log/Log.h"

namespace CC2500 {
    namespace { // private
        constexpr auto LogTag = "CC2500";

        const SPISettings spiSettings = SPISettings(10000000, MSBFIRST, SPI_MODE0);


        constexpr uint8_t BurstMask = 0x40;

        constexpr uint8_t powerAmplifierTable[8] = {
                0xC5, // -12dbm
                0x97, // -10dbm
                0x6E, // -8dbm
                0x7F, // -6dbm
                0xA9, // -4dbm
                0xBB, // -2dbm
                0xFE, // 0dbm
                0xFF  // 1.5dbm
        };
    }

    CC2500::CC2500(SPIClass &spi, uint32_t csPin) : device(new BusIO::SPIDevice(&spi, spiSettings, csPin)) {}

    void CC2500::readFifo(uint8_t *buffer, size_t length) {
        this->device->burstRead(Registers::RXFIFO | BurstMask, buffer, length);
    }

    void CC2500::writeFifo(uint8_t *buffer, size_t length) {
        this->strobe(StrobeCommands::FlushTXFIFO);
        this->device->burstWrite(Registers::TXFIFO | BurstMask, buffer, length);
        this->strobe(StrobeCommands::EnableTX);
    }

    void CC2500::readRegisterMulti(uint8_t address, uint8_t *data, uint8_t length) {
        this->device->burstRead(address, data, length);
    }

    void CC2500::writeRegisterMulti(uint8_t address, uint8_t *data, uint8_t length) {
        this->device->burstWrite(address, data, length);
    }

    uint8_t CC2500::readRegister(uint8_t address) {
        return this->device->readRegister(address);
    }

    void CC2500::strobe(uint8_t address) {
        this->device->strobeRegister(address);
    }

    void CC2500::writeRegister(uint8_t address, uint8_t data) {
        this->device->writeRegister(address, data);
    }

    void CC2500::setPower(uint8_t power) {
        if (power > 7) power = 7;
        this->writeRegister(Registers::PATABLE, powerAmplifierTable[power]);
    }

    void CC2500::reset() {
        this->strobe(StrobeCommands::Reset);
        delayMicroseconds(1000);
        bool reset = this->readRegister(Registers::FrequencyControl1) == 0xC4;
        if (!reset) Log::warning(LogTag, "Reset failed");
    }
}
