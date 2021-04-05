#pragma once

#include <Arduino.h>
#include "rc/RC.h"
#include "hardware/AbstractSerialPort.h"

namespace RcDrivers {
    class IBUSDriver : public RC::Driver {
    public:
        explicit IBUSDriver(IO::SerialPort& uart);
        bool getFrame(RC::ChannelsMicroseconds& channels) override;
        bool shouldTryToGetFrame() override;

    private:
        enum class State {
            GetLength,
            GetData,
            GetChecksumLow,
            GetChecksumHigh,
            Discard
        };

        static constexpr size_t ChannelCount = 14;
        static constexpr size_t MaxMessageLength = 32;

        IO::SerialPort& uart;
        uint32_t lastByteTime = 0;
        State state = State::GetLength;
        uint8_t messageBuffer[MaxMessageLength] {};
        uint8_t messageBufferOffset = 0;
        uint8_t messageBufferLength = 0;
        uint16_t checksum = 0;
        uint8_t checksumLowByte = 0;
    };
}
