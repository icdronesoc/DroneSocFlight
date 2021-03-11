#include "DShot.h"
#include "log/Log.h"
#include "utils/Maths.h"

namespace DShot {
    namespace { // private
        const auto LogTag = "DShot";

        constexpr uint16_t MinThrottle = 48;
        constexpr uint16_t MaxThrottle = 2047;
        constexpr uint16_t BidirectionalForwardMinThrottle = MinThrottle;
        constexpr uint16_t BidirectionalForwardMaxThrottle = 1047;
        constexpr uint16_t BidirectionalReverseMinThrottle = 1049;
        constexpr uint16_t BidirectionalReverseMaxThrottle = MaxThrottle;

        uint16_t assemblePacket(uint16_t rawValue, bool requestTelemetry) {
            uint16_t packet = rawValue << 5;
            if (requestTelemetry) packet |= 0b0000000000010000;

            uint16_t checksum = 0;
            for (int i = 0; i < 3; i++) {
                checksum ^= packet >> (4 * i);
            }
            packet |= (checksum & 0xF);

            return packet;
        }
    }

    etl::vector<Output*, MaxDShotOutputs> allOutputs;

    Output* createOutput(uint32_t pin, Speed speed, bool isBidirectional) {
        if (allOutputs.full()) {
            Log::error(LogTag, "Cannot create DShot output: maximum number of outputs reached.");
            return nullptr;
        }

        auto driver = createDriver(pin, speed);
        if (driver == nullptr) {
            Log::error(LogTag, "Cannot create DShot output: Error initializing driver.");
            return nullptr;
        }

        auto newOutput = new Output(*driver, isBidirectional);
        allOutputs.push_back(newOutput);
        return newOutput;
    }

    void sendCommandToAllOutputs(Command command) {
        for (auto& output : allOutputs) {
            output->sendCommand(command);
        }
    }

    Output::Output(Driver &driver, bool isBidirectional) : driver(driver), isBidirectional(isBidirectional) {}

    void Output::setThrottle(double throttle) {
        uint16_t rawValue;
        if (throttle == 0) {
            rawValue = static_cast<uint16_t>(Command::MOTOR_STOP);
        } else if (this->isBidirectional) {
            if (throttle > 0) {
                rawValue = Maths::map(throttle, 0, 1, BidirectionalForwardMinThrottle, BidirectionalForwardMaxThrottle);
            } else {
                rawValue = Maths::map(throttle, 0, -1, BidirectionalReverseMinThrottle, BidirectionalReverseMaxThrottle);
            }
        } else {
            rawValue = Maths::map(throttle, 0, 1, MinThrottle, MaxThrottle);
        }
        this->driver.sendPacket(assemblePacket(rawValue, false));
    }

    void Output::sendCommand(Command command) {
        this->driver.sendPacket(assemblePacket(static_cast<uint16_t>(command), false));
    }
}
