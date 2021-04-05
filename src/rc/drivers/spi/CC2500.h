#pragma once

#include <Arduino.h>
#include <SPI.h>
#include "hardware/BusIO.h"

namespace CC2500 {
    namespace Registers {
        constexpr uint8_t IOConfiguration2 = 0x00;
        constexpr uint8_t IOConfiguration1 = 0x01;
        constexpr uint8_t IOConfiguration0 = 0x02;
        constexpr uint8_t FIFOThresholds = 0x03;
        constexpr uint8_t Sync1 = 0x04;
        constexpr uint8_t Sync0 = 0x05;
        constexpr uint8_t PacketLength = 0x06;
        constexpr uint8_t PacketAutomationControl1 = 0x07;
        constexpr uint8_t PacketAutomationControl0 = 0x08;
        constexpr uint8_t Address = 0x09;
        constexpr uint8_t ChannelNumber = 0x0A;
        constexpr uint8_t FrequencySynthesizerControl1 = 0x0B;
        constexpr uint8_t FrequencySynthesizerControl0 = 0x0C;
        constexpr uint8_t FrequencyControl2 = 0x0D;
        constexpr uint8_t FrequencyControl1 = 0x0E;
        constexpr uint8_t FrequencyControl0 = 0x0F;
        constexpr uint8_t ModemConfiguration4 = 0x10;
        constexpr uint8_t ModemConfiguration3 = 0x11;
        constexpr uint8_t ModemConfiguration2 = 0x12;
        constexpr uint8_t ModemConfiguration1 = 0x13;
        constexpr uint8_t ModemConfiguration0 = 0x14;
        constexpr uint8_t ModemDeviationSetting = 0x15;
        constexpr uint8_t ControlStateMachineConfig2 = 0x16;
        constexpr uint8_t ControlStateMachineConfig1 = 0x17;
        constexpr uint8_t ControlStateMachineConfig0 = 0x18;
        constexpr uint8_t FrequencyOffsetCompensationConfig = 0x19;
        constexpr uint8_t BitSynchronizationConfig = 0x1A;
        constexpr uint8_t AGCControl2 = 0x1B;
        constexpr uint8_t AGCControl1 = 0x1C;
        constexpr uint8_t AGCControl0 = 0x1D;
        constexpr uint8_t WakeOnRadioEventTimeout1 = 0x1E;
        constexpr uint8_t WakeOnRadioEventTimeout0 = 0x1F;
        constexpr uint8_t WakeOnRadioControl = 0x20;
        constexpr uint8_t FrontEndRxConfig = 0x21;
        constexpr uint8_t FrontEndTxConfig = 0x22;
        constexpr uint8_t FrequencySynthesizerCalibration3 = 0x23;
        constexpr uint8_t FrequencySynthesizerCalibration2 = 0x24;
        constexpr uint8_t FrequencySynthesizerCalibration1 = 0x25;
        constexpr uint8_t FrequencySynthesizerCalibration0 = 0x26;
        constexpr uint8_t RCOscillatorConfig1 = 0x27;
        constexpr uint8_t RCOscillatorConfig0 = 0x28;
        constexpr uint8_t FrequencySynthesizerTest = 0x29;
        constexpr uint8_t ProductionTest = 0x2A;
        constexpr uint8_t AGCTest = 0x2B;
        constexpr uint8_t Test2 = 0x2C;
        constexpr uint8_t Test1 = 0x2D;
        constexpr uint8_t Test0 = 0x2E;

        // Status registers
        constexpr uint8_t PartNumber = 0x30;
        constexpr uint8_t VersionNumber = 0x31;
        constexpr uint8_t FrequencyOffsetEstimate = 0x32;
        constexpr uint8_t LinkQualityEstimate = 0x33;
        constexpr uint8_t RSSI = 0x34;
        constexpr uint8_t ControlStateMachineState = 0x35;
        constexpr uint8_t WakeOnRadioTimer1 = 0x36;
        constexpr uint8_t WakeOnRadioTimer0 = 0x37;
        constexpr uint8_t PacketStatus = 0x38;
        constexpr uint8_t VCO_VC_DAC = 0x39;
        constexpr uint8_t TXFIFOBytes = 0x3A;
        constexpr uint8_t RXFIFOBytes = 0x3B;

        // Multi byte memory locations
        constexpr uint8_t PATABLE = 0x3E;
        constexpr uint8_t TXFIFO = 0x3F;
        constexpr uint8_t RXFIFO = 0x3F;
    }

    namespace StrobeCommands {
        constexpr uint8_t Reset = 0x30;
        // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1).
        // If in RX/TX: Go to a wait state where only the synthesizer is
        // running (for quick RX / TX turnaround).
        constexpr uint8_t CC2500_SFSTXON = 0x31;
        constexpr uint8_t CrystalOscillatorOff = 0x32;
        constexpr uint8_t CalibrateFrequencySynthesizer = 0x33;
        constexpr uint8_t EnableRX = 0x34;
        constexpr uint8_t EnableTX = 0x35;
        constexpr uint8_t Idle = 0x36;
        constexpr uint8_t CC2500_SAFC = 0x37; // Perform AFC adjustment of the frequency synthesizer
        constexpr uint8_t CC2500_SWOR = 0x38; // Start automatic RX polling sequence (Wake-on-Radio)
        constexpr uint8_t CC2500_SPWD = 0x39; // Enter power down mode when CSn goes high.
        constexpr uint8_t FlushRXFIFO = 0x3A;
        constexpr uint8_t FlushTXFIFO = 0x3B;
        constexpr uint8_t ResetRTC = 0x3C;
        constexpr uint8_t NOP = 0x3D;
    }

    class CC2500 {
    public:
        CC2500(SPIClass &spi, uint32_t csPin);

        void readFifo(uint8_t *buffer, size_t length);

        void writeFifo(uint8_t *buffer, size_t length);

        void readRegisterMulti(uint8_t address, uint8_t *data, uint8_t length);

        void writeRegisterMulti(uint8_t address, uint8_t *data, uint8_t length);

        uint8_t readRegister(uint8_t address);

        void strobe(uint8_t address);

        void writeRegister(uint8_t address, uint8_t data);

        /**
         * Sets the CC2500 Power level
         * @param power The power level, from 0 to 7 inclusive
         */
        void setPower(uint8_t power);

        void reset();

    private:
        BusIO::SPIDevice *device;
    };
}
