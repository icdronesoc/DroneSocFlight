#include "RC.h"
#include "config/Config.h"
#include "hardware/IO.h"
#include "scheduler/Scheduler.h"
#include "rc/drivers/AllDrivers.h"

namespace RC {
    namespace { // private
        Driver* driver = nullptr;
        uint32_t lastFrameTimeMs = 0;

        bool shouldTryToGetFrame() {
            if (driver == nullptr) return false;
            return driver->shouldTryToGetFrame();
        }

        const Scheduler::Name RCTaskName = "RC RX";
        void tryGetFrame() {
            if (driver == nullptr) return;
            if (driver->getFrame(channels)) {
                lastFrameTimeMs = millis();
            }
        }
    }

    Channels channels;

    void initialize() {
        // Choose driver
        switch (Config::hardwareConfig.rcConfig.which_driverConfig) {
            case RCConfig_crossfire_tag: {
                auto uartIndex = Config::hardwareConfig.rcConfig.driverConfig.ibus.uartIndex;
                if (uartIndex < IO::UARTs.size() && IO::UARTs[uartIndex] != nullptr) {
                    driver = new RcDrivers::CrossfireDriver(*IO::UARTs[uartIndex]);
                }
                break;
            }
            case RCConfig_ibus_tag: {
                auto uartIndex = Config::hardwareConfig.rcConfig.driverConfig.ibus.uartIndex;
                if (uartIndex < IO::UARTs.size() && IO::UARTs[uartIndex] != nullptr) {
                    driver = new RcDrivers::IBUSDriver(*IO::UARTs[uartIndex]);
                }
                break;
            }
        }

        if (driver != nullptr) {
            // Initialize channels
            for (size_t i = 0; i < min(driver->channelCount, MaxChannelCount); i++) {
                channels.push_back(0);
            }

            // Setup get RC data task
            auto task = new Scheduler::Task(tryGetFrame, RCTaskName);
            auto runner = new Scheduler::AdHocTaskRunner(task, shouldTryToGetFrame);
            Scheduler::addTaskRunner(runner);
        }
    }

    uint32_t timeSinceLastFrame() {
        return millis() - lastFrameTimeMs;
    }
}
