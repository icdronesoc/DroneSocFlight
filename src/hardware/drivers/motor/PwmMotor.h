#pragma once

#include <Arduino.h>
#include "hardware/AbstractHardware.h"

namespace MotorDrivers {
    class PwmMotor : public Motor {
    public:
        void initialize() override;
        void setOutput(int16_t output) override;
    };
}
