#include <Arduino.h>

#include "hardware/McuHardware.h"
#include "hardware/Hardware.h"

void setup() {
    McuHardware::initialize();
    Config::loadConfiguration();
    Hardware::initialize();
}

void loop() {

}
