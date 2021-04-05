/*
 * Based on Betaflight's implementation of the CRSF protocol, licensed under GPLv3:
 * https://github.com/betaflight/betaflight/blob/4.2.8/src/main/rx/crsf.c
 * https://github.com/betaflight/betaflight/blob/4.2.8/src/main/rx/crsf.h
 * https://github.com/betaflight/betaflight/blob/4.2.8/src/main/rx/crsf_protocol.h
 * https://github.com/betaflight/betaflight/blob/4.2.8/src/main/telemetry/crsf.c
 * https://github.com/betaflight/betaflight/blob/4.2.8/src/main/telemetry/crsf.h
 */

#include "Crossfire.h"
#include "utils/CRC.h"

namespace RcDrivers {
    namespace { // private
        constexpr size_t ChannelCount = 16;

        constexpr uint32_t Baud = 420000;
        constexpr uint32_t MaxFrameTime = 1100; // Microseconds
        constexpr size_t HeaderLength = 3; // Device Address + Frame Length + Type
        constexpr uint8_t TelemetrySyncByte = 0xC8;
        constexpr uint8_t DeviceInfoVersion = 0x01;

        namespace FrameLengths {
            constexpr uint8_t Address = 1; // Length of Address field
            constexpr uint8_t FrameLength = 1; // Length of Frame Length field
            constexpr uint8_t Type = 1; // Length of Type field
            constexpr uint8_t CRCField = 1; // Length of CRC field
            constexpr uint8_t TypeAndCRC = Type + CRCField; // Length of Type and CRC fields
            constexpr uint8_t ExtendedTypeAndCRC = 2 + TypeAndCRC; // Length of Extended Destination / Origin, TYPE and CRC fields
            constexpr uint8_t Headers = 4; // Length of all non-payload fields
        }

        namespace FrameTypes {
            constexpr uint8_t GPS = 0x02;
            constexpr uint8_t BatterySensor = 0x08;
            constexpr uint8_t LinkStatistics = 0x14;
            constexpr uint8_t RCChannels = 0x16;
            constexpr uint8_t Attitude = 0x1E;
            constexpr uint8_t FlightMode = 0x21;
            constexpr uint8_t DevicePing = 0x28;
            constexpr uint8_t DeviceInfo = 0x29;
            constexpr uint8_t ParameterSettingsEntry = 0x2B;
            constexpr uint8_t ParameterRead = 0x2C;
            constexpr uint8_t ParameterWrite = 0x2D;
            constexpr uint8_t Command = 0x32;
            constexpr uint8_t MSPRequest = 0x7A;
            constexpr uint8_t MSPResponse = 0x7B;
            constexpr uint8_t MSPWrite = 0x7C;
            constexpr uint8_t DisplayportCommand = 0x7D;
        }

        namespace FrameSizes {
            constexpr uint8_t TX_MSP = 58;
            constexpr uint8_t RX_MSP = 8;
            constexpr uint8_t OriginAndDestination = 2;
        }

        namespace Addresses {
            constexpr uint8_t Broadcast = 0x00;
            constexpr uint8_t USB = 0x10;
            constexpr uint8_t TBSCorePNPPro = 0x80;
            constexpr uint8_t Reserved1 = 0x8A;
            constexpr uint8_t CurrentSensor = 0xC0;
            constexpr uint8_t GPS = 0xC2;
            constexpr uint8_t TBSBlackbox = 0xC4;
            constexpr uint8_t FlightController = 0xC8;
            constexpr uint8_t Reserved2 = 0xCA;
            constexpr uint8_t RaceTag = 0xCC;
            constexpr uint8_t RadioTransmitter = 0xEA;
            constexpr uint8_t CrossfireReceiver = 0xEC;
            constexpr uint8_t CrossfireTransmitter = 0xEE;
        }

        namespace PayloadSizes {
            constexpr uint8_t GPS = 15;
            constexpr uint8_t BatterySensor = 8;
            constexpr uint8_t LinkStatistics = 10;
            constexpr uint8_t RCChannels = 22;
            constexpr uint8_t Attitude = 6;
        }

        struct PackedRCChannels {
            unsigned int channel0 : 11;
            unsigned int channel1 : 11;
            unsigned int channel2 : 11;
            unsigned int channel3 : 11;
            unsigned int channel4 : 11;
            unsigned int channel5 : 11;
            unsigned int channel6 : 11;
            unsigned int channel7 : 11;
            unsigned int channel8 : 11;
            unsigned int channel9 : 11;
            unsigned int channel10 : 11;
            unsigned int channel11 : 11;
            unsigned int channel12 : 11;
            unsigned int channel13 : 11;
            unsigned int channel14 : 11;
            unsigned int channel15 : 11;
        } __attribute__ ((__packed__));

        struct LinkStatistics {
            uint8_t uplinkRSSI1;
            uint8_t uplinkRSSI2;
            uint8_t uplinkLinkQuality;
            int8_t uplinkSNR;
            uint8_t activeAntenna;
            uint8_t rfMode;
            uint8_t uplinkTransmitPower;
            uint8_t downlinkRSSI;
            uint8_t downlinkLinkQuality;
            int8_t downlinkSNR;
        };
    }

    CrossfireDriver::CrossfireDriver(IO::SerialPort& uart) : RC::Driver(ChannelCount), uart(uart) {
        this->uart.begin(Baud);
    }

    bool CrossfireDriver::getFrame(RC::ChannelsMicroseconds& channels) {
        bool frameReceived = false;
        while (this->uart->available()) {
            // Carry on processing any serial data even if we've already received a frame
            if (this->handleData(this->uart->read(), channels)) frameReceived = true;
        }
        return frameReceived;
    }

    bool CrossfireDriver::shouldTryToGetFrame() {
        return this->uart->available() > 0;
    }

    bool CrossfireDriver::handleData(uint8_t newData, RC::ChannelsMicroseconds& channels) {
        bool rcFrameReceived = false;
        const uint32_t currentTime = micros();

        if (this->frameStartTime - currentTime > MaxFrameTime) this->frameBufferIndex = 0;
        if (this->frameBufferIndex == 0) this->frameStartTime = currentTime;

        const size_t frameLength = this->frameBufferIndex < HeaderLength ? 5 : this->frameBuffer[1] + FrameLengths::Address + FrameLengths::FrameLength;

        if (this->frameBufferIndex < frameLength && frameLength <= MaxFrameLength) {
            this->frameBuffer[this->frameBufferIndex++] = newData;
            if (this->frameBufferIndex >= frameLength) {
                this->frameBufferIndex = 0;
                const uint8_t crc = this->calculateCRC();
                if (crc == this->frameBuffer[frameLength - 1]) {
                    switch (this->frameBuffer[2]) { // Frame Type
                        case FrameTypes::RCChannels: {
                            if (this->frameBuffer[0] == Addresses::FlightController && this->frameBuffer[1] == PayloadSizes::RCChannels) {
                                // We got an RC frame! TODO translate crossfire values to microseconds
                                const auto rcChannels = reinterpret_cast<PackedRCChannels*>(this->frameBuffer + HeaderLength);
                                channels[0] = rcChannels->channel0;
                                channels[1] = rcChannels->channel1;
                                channels[2] = rcChannels->channel2;
                                channels[3] = rcChannels->channel3;
                                channels[4] = rcChannels->channel4;
                                channels[5] = rcChannels->channel5;
                                channels[6] = rcChannels->channel6;
                                channels[7] = rcChannels->channel7;
                                channels[8] = rcChannels->channel8;
                                channels[9] = rcChannels->channel9;
                                channels[10] = rcChannels->channel10;
                                channels[11] = rcChannels->channel11;
                                channels[12] = rcChannels->channel12;
                                channels[13] = rcChannels->channel13;
                                channels[14] = rcChannels->channel14;
                                channels[15] = rcChannels->channel15;
                                rcFrameReceived = true;
                            }
                            break;
                        }

                        case FrameTypes::DevicePing: {
                            this->initializeTelemetryFrame();
                            this->telemetryFrameBuffer[telemetryFrameLength++] = 0;
                            this->telemetryFrameBuffer[telemetryFrameLength++] = FrameTypes::DeviceInfo;
                            this->telemetryFrameBuffer[telemetryFrameLength++] = Addresses::RadioTransmitter;
                            this->telemetryFrameBuffer[telemetryFrameLength++] = Addresses::FlightController;
                            auto res = snprintf(reinterpret_cast<char*>(this->telemetryFrameBuffer + telemetryFrameLength), 30,"%s %s: %s", "DroneSocFlight", "v0.0.1", "TEST");
                            if (res > 0) {
                                this->telemetryFrameLength += res + 1; // +1 for null terminator
                                for (size_t i = 0; i < 12; i++) {
                                    this->telemetryFrameBuffer[telemetryFrameLength++] = 0;
                                }
                                this->telemetryFrameBuffer[telemetryFrameLength++] = 0;
                                this->telemetryFrameBuffer[telemetryFrameLength++] = DeviceInfoVersion;
                                this->finalizeTelemetryFrame();
                            }
                            break;
                        }

                        case FrameTypes::LinkStatistics: {
                            if (this->frameBuffer[0] == Addresses::FlightController && this->frameBuffer[1] == FrameSizes::OriginAndDestination + PayloadSizes::LinkStatistics) {
                                const auto linkStatistics = reinterpret_cast<LinkStatistics*>(this->frameBuffer + HeaderLength);
                                // TODO use link statistics
                            }
                            break;
                        }
                    }
                }
            }
        }

        return rcFrameReceived;
    }

    uint8_t CrossfireDriver::calculateCRC() {
        // CRC includes type and payload
        uint8_t crc = CRCUtils::crc8_dvb_s2(0, this->frameBuffer[2]);
        for (uint8_t i = 0; i < this->frameBuffer[1] - FrameLengths::TypeAndCRC; i++) {
            crc = CRCUtils::crc8_dvb_s2(crc, this->frameBuffer[HeaderLength + i]);
        }
        return crc;
    }

    void CrossfireDriver::initializeTelemetryFrame() {
        this->telemetryFrameLength = 1;
        this->telemetryFrameBuffer[0] = TelemetrySyncByte;
    }

    void CrossfireDriver::finalizeTelemetryFrame() {
        uint8_t crc = 0;
        for (size_t i = 2; i < this->telemetryFrameLength; i++) {
            crc = CRCUtils::crc8_dvb_s2(crc, this->telemetryFrameBuffer[i]);
        }
        this->telemetryFrameBuffer[this->telemetryFrameLength++] = crc;
        this->uart->write(this->telemetryFrameBuffer, this->telemetryFrameLength);
    }
}
