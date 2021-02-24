#include <Arduino.h>

#include "hardware/IO.h"
#include "hardware/Hardware.h"
#include "rc/RC.h"

void setup() {
    Config::loadConfig();
    IO::initialize();
    Hardware::initialize();
    RC::initialize();
}

void loop() {
    IO::resetWatchdogTimer();
}
