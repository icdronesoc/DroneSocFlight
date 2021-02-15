#include <Arduino.h>

#include "hardware/IO.h"
#include "hardware/Hardware.h"

void setup() {
    Config::loadConfig();
    IO::initialize();
    Hardware::initialize();
}

void loop() {
    IO::resetWatchdogTimer();
}
