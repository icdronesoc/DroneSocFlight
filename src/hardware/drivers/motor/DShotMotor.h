#pragma once

#include <Arduino.h>
#include "hardware/AbstractHardware.h"

namespace MotorDrivers {
    class DShotMotor : public Motor {
    public:
        void initialize() override;
        void setOutput(int16_t output) override;
    };
}
