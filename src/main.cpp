#include <Arduino.h>

#include "hardware/McuHardware.h"
#include "config/Config.pb.h"

void setup() {
    McuHardware::initialize();
}

void loop() {
    HardwareConfiguration hardwareConfiguration = HardwareConfiguration_init_zero;
}
