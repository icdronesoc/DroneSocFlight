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

#include "hardware/drivers/motor/DShotMotor.h"
Hardware::Motor* motor = nullptr;

void setup() {
    // Config must be loaded first, then IO, then Log, then Hardware. After this it doesn't really matter.
    Config::loadConfig();
    IO::initialize();
    Log::initialize();
    Hardware::initialize();
    RC::initialize();
    Mixer::initialize();
    AxisControllers::initialize();
    FlightControlTasks::initialize();
    Log::info(LogTag, "Startup Complete!");

    motor = new MotorDrivers::DShotMotor(*DShot::createOutput(LED_BUILTIN, DShot::Speed::DShot300, false));
}

int i = 0;

void loop() {
    IO::resetWatchdogTimer();
//    Scheduler::loop();
    Log::info(LogTag, "Writing DShot packet...");
    motor->setOutput((i++) % 2048);
    delay(1);
}
