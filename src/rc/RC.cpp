#include "RC.h"
#include "config/Config.h"
#include "hardware/IO.h"
#include "scheduler/Scheduler.h"
#include "rc/drivers/AllDrivers.h"
#include "log/Log.h"

namespace RC {
    namespace { // private
        const auto LogTag = "RC";

        Driver *driver = nullptr;
        uint32_t lastFrameTimeMs = 0;

        bool shouldTryToGetFrame() {
            // driver guaranteed not to be null because otherwise the task would never have been scheduled.
            return driver->shouldTryToGetFrame();
        }

        const Scheduler::Name RCTaskName = "RC Receiver";

        ChannelsMicroseconds channelsMicroseconds;

        void tryGetFrame() {
            // driver guaranteed not to be null because otherwise the task would never have been scheduled.
            if (driver->getFrame(channelsMicroseconds)) {
                lastFrameTimeMs = millis();
            }

            // TODO translate from microseconds to -1 to 1 and store in channels
        }
    }

    Channels channels;

    void initialize() {
        Log::info(LogTag, "Initializing RC Driver");
        // Choose driver
        if (Config::config.has_rcConfig) {
            switch (Config::config.rcConfig.which_driverConfig) {
                case RCConfig_crossfire_tag: {
                    if (Config::config.rcConfig.driverConfig.crossfire.has_uart) {
                        auto uart = IO::takeUart(Config::config.rcConfig.driverConfig.crossfire.uart.name);
                        if (uart != nullptr) {
                            driver = new RcDrivers::CrossfireDriver(*uart);
                        } else {
                            Log::error(LogTag, "RC Driver UART not valid.");
                        }
                    } else {
                        Log::error(LogTag, "RC Driver UART not configured.");
                    }
                    break;
                }
                case RCConfig_ibus_tag: {
                    if (Config::config.rcConfig.driverConfig.ibus.has_uart) {
                        auto uart = IO::takeUart(Config::config.rcConfig.driverConfig.ibus.uart.name);
                        if (uart != nullptr) {
                            driver = new RcDrivers::IBUSDriver(*uart);
                        } else {
                            Log::error(LogTag, "RC Driver UART not valid.");
                        }
                    } else {
                        Log::error(LogTag, "RC Driver UART not configured.");
                    }
                    break;
                }
                default:
                    Log::error(LogTag, "RC Driver Config type invalid.");
            }
        } else {
            Log::warning(LogTag, "No RC Driver configured.");
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

            Log::info(LogTag, "RC Driver initialization complete.");
        }
    }

    uint32_t timeSinceLastFrame() {
        return millis() - lastFrameTimeMs;
    }
}
