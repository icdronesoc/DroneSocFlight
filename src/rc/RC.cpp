#include "RC.h"
#include "config/Config.h"
#include "hardware/IO.h"
#include "rc/drivers/AllDrivers.h"

namespace RC {
    Channels channels;

    namespace { // private
        Driver* driver = nullptr;
        uint32_t lastFrameTimeMs = 0;
    }

    void initialize() {
        // Choose driver
        switch (Config::hardwareConfig.rcConfig.which_driverConfig) {
            case RCConfig_ibus_tag:
                auto uartIndex = Config::hardwareConfig.rcConfig.driverConfig.ibus.uartIndex;
                if (uartIndex < IO::UARTs.size()) {
                    driver = new RcDrivers::IBUSDriver(IO::UARTs[uartIndex]);
                }
                break;
        }

        // Initialize channels
        if (driver != nullptr) {
            for (size_t i = 0; i < min(driver->channelCount, MaxChannelCount); i++) {
                channels.push_back(0);
            }
        }
    }

    void loopTask() {
        if (driver == nullptr) return;
        if (driver->getFrame(channels)) {
            lastFrameTimeMs = millis();
        }
    }

    uint32_t timeSinceLastFrame() {
        return millis() - lastFrameTimeMs;
    }
}
