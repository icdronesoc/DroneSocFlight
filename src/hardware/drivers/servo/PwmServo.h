#pragma once

#include <Arduino.h>
#include "hardware/AbstractHardware.h"

namespace ServoDrivers {
    class PwmServo : public Servo {
    public:
        void initialize() override;
        void setOutput(int16_t output) override;
    };
}
