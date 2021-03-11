#include <Arduino.h>

#include "hardware/IO.h"
#include "log/Log.h"
#include "hardware/Hardware.h"
#include "rc/RC.h"
#include "scheduler/Scheduler.h"
#include "flight/AxisControllers.h"
#include "flight/FlightControlTasks.h"
#include "flight/Mixer.h"

namespace { // private
    const auto LogTag = "Main";
}

void setup() {
    // Config must be loaded first, then IO, then Log, then Hardware. After this it doesn't really matter.
    Config::loadConfig();
    IO::initialize();
    Log::initialize();
    Log::info(LogTag, "Beginning initialization...");
    Hardware::initialize();
    RC::initialize();
    Mixer::initialize();
    AxisControllers::initialize();
    FlightControlTasks::initialize();
    Log::info(LogTag, "Configuration Complete!");
}

void loop() {
    IO::resetWatchdogTimer();
    Scheduler::loop();
}
