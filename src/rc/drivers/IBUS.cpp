/*
 * Based on IBusBM's implementation of the IBUS protocol, licensed under GPLv3:
 * https://github.com/bmellink/IBusBM/blob/v1.1.5/src/IBusBM.h
 * https://github.com/bmellink/IBusBM/blob/v1.1.5/src/IBusBM.cpp
 */

#include "IBUS.h"

namespace RcDrivers {
    namespace { // private
        constexpr uint8_t PROTOCOL_OVERHEAD = 3; // Packet is <len><cmd><data....><chkl><chkh>
        constexpr uint8_t PROTOCOL_TIMEGAP = 3; // Packets are received very ~7ms so use ~half that for the gap
        constexpr uint8_t PROTOCOL_COMMAND_SET_OUTPUT = 0x40;
    }

    IBUSDriver::IBUSDriver(IO::SerialPort& uart) : RC::Driver(ChannelCount), uart(uart) {
        this->uart.begin(115200);
    }

    bool IBUSDriver::getFrame(RC::ChannelsMicroseconds& channels) {
        bool channelsUpdated = false;
        while (this->uart->available() > 0) {
            uint32_t now = millis();
            if (now - this->lastByteTime >= PROTOCOL_TIMEGAP) this->state = State::GetLength;
            this->lastByteTime = now;

            uint8_t newByte = this->uart->read();
            switch (this->state) {
                case State::GetLength:
                    if (newByte <= MaxMessageLength && newByte > PROTOCOL_OVERHEAD) {
                        this->messageBufferOffset = 0;
                        this->messageBufferLength = newByte - PROTOCOL_OVERHEAD;
                        this->checksum = 0xFFFF - newByte;
                        this->state = State::GetData;
                    } else {
                        this->state = State::Discard;
                    }
                    break;

                case State::GetData:
                    this->messageBuffer[this->messageBufferOffset++] = newByte;
                    this->checksum -= newByte;
                    if (this->messageBufferOffset == this->messageBufferLength) state = State::GetChecksumLow;
                    break;

                case State::GetChecksumLow:
                    this->checksumLowByte = newByte;
                    state = State::GetChecksumHigh;
                    break;

                case State::GetChecksumHigh:
                    if (this->checksum == (newByte << 8) + this->checksumLowByte) {
                        if (this->messageBuffer[0] == PROTOCOL_COMMAND_SET_OUTPUT) {
                            uint8_t* channelsBuffer = this->messageBuffer + 1;
                            for (size_t i = 0; i < ChannelCount; i++) channels[i] = channelsBuffer[2*i] | (channelsBuffer[(2*i)+1] << 8);
                            channelsUpdated = true;
                        }
                    }
                    this->state = State::Discard;
                    break;

                case State::Discard:
                default:
                    break;
            }
        }
        return channelsUpdated;
    }

    bool IBUSDriver::shouldTryToGetFrame() {
        return this->uart->available() > 0;
    }
}
