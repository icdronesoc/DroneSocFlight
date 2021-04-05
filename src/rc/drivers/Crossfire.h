#pragma once

#include <Arduino.h>
#include "rc/RC.h"
#include "hardware/AbstractSerialPort.h"

namespace RcDrivers {
    class CrossfireDriver : public RC::Driver {
    public:
        explicit CrossfireDriver(IO::SerialPort& uart);
        bool getFrame(RC::ChannelsMicroseconds& channels) override;
        bool shouldTryToGetFrame() override;

    private:
        IO::SerialPort& uart;
        const static size_t MaxFrameLength = 64;
        /**
         * Index 0 = Device Address
         * Index 1 = Frame Length
         * Index 2 = Frame Type
         * Index 3...FrameLength+3 = Payload
         * Index FrameLength+4 = CRC
         */
        uint8_t frameBuffer[MaxFrameLength] {};
        size_t frameBufferIndex = 0;
        /**
         * The time at which the first byte of the current frame was received, in microseconds.
         */
        uint32_t frameStartTime = 0;

        // TODO send as we go, buffer is unnecessary
        uint8_t telemetryFrameBuffer[MaxFrameLength] {};
        size_t telemetryFrameLength = 0;

        /**
         * @param newData New data read from the UART
         * @param channels The RC channel data that should be updated if a frame is received
         * @return Whether a frame was received
         */
        bool handleData(uint8_t newData, RC::ChannelsMicroseconds& channels);
        uint8_t calculateCRC();
        void initializeTelemetryFrame();
        /**
         * Finalizes and sends the Telemetry frame.
         */
        void finalizeTelemetryFrame();
    };
}
