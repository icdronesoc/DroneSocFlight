#include <Arduino.h>

#include "hardware/IO.h"
#include "debug/DebugInterface.h"
#include "hardware/Hardware.h"
#include "rc/RC.h"
#include "scheduler/Scheduler.h"
#include "flight/AxisControllers.h"
#include "flight/FlightControlTasks.h"
#include "flight/Mixer.h"

void setup() {
    // Config must be loaded first, then IO, then Debug, then Hardware. After this it doesn't really matter.
    Config::loadConfig();
    IO::initialize();
    Debug::initialize();
    Hardware::initialize();
    RC::initialize();
    Mixer::initialize();
    AxisControllers::initialize();
    FlightControlTasks::initialize();
    Debug::info("Configuration Complete!");
}

void loop() {
    IO::resetWatchdogTimer();
    Scheduler::loop();
}
