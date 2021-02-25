#include <Arduino.h>

#include "hardware/IO.h"
#include "hardware/Hardware.h"
#include "rc/RC.h"
#include "scheduler/Scheduler.h"
#include "flight/FlightControlTasks.h"
#include "flight/Mixer.h"

void setup() {
    Config::loadConfig();
    IO::initialize();
    Hardware::initialize();
    RC::initialize();
    Mixer::initialize();
    FlightControlTasks::initialize();
}

void loop() {
    IO::resetWatchdogTimer();
    Scheduler::loop();
}
