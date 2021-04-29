#include <Arduino.h>

#include "hardware/IO.h"
#include "log/Log.h"
#include "hardware/Hardware.h"
#include "hardware/target/target.h"
#include "rc/RC.h"
#include "scheduler/Scheduler.h"
#include "flight/AxisControllers.h"
#include "flight/FlightControlTasks.h"
#include "flight/Mixer.h"

namespace { // private
    const auto LogTag = "Main";
}

#include "hardware/drivers/motor/DShotMotor.h"
Hardware::Motor* motor1 = nullptr;
Hardware::Motor* motor2 = nullptr;

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

    motor1 = new MotorDrivers::DShotMotor(*DShot::createOutput(25, DShot::Speed::DShot300, false, true));
//    motor2 = new MotorDrivers::DShotMotor(*DShot::createOutput(26, DShot::Speed::DShot300, false, true));
    pinMode(26, OUTPUT);
    digitalWrite(26, LOW);
}

int i = 0;

void loop() {
    Target::resetWatchdogTimer();
//    Scheduler::loop();
    Log::info(LogTag, "Writing DShot packet...");
    auto value = (i++) % 2048;
    if (motor1 != nullptr) motor1->setOutput(value);
    if (motor2 != nullptr) motor2->setOutput(value);
    delay(1);
}
