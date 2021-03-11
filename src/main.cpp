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
    Log::info(LogTag, "Loading Config");
    Config::loadConfig();
    Log::info(LogTag, "Initializing IO");
    IO::initialize();
    Log::info(LogTag, "Initializing Log");
    Log::initialize();
    Log::info(LogTag, "Initializing Hardware");
    Hardware::initialize();
    Log::info(LogTag, "Initializing RC");
    RC::initialize();
    Log::info(LogTag, "Initializing Mixer");
    Mixer::initialize();
    Log::info(LogTag, "Initializing Axis Controllers");
    AxisControllers::initialize();
    Log::info(LogTag, "Initializing Flight Control Tasks");
    FlightControlTasks::initialize();
    Log::info(LogTag, "Configuration Complete!");
}

void loop() {
    IO::resetWatchdogTimer();
    Scheduler::loop();
}
