#pragma once

#include <Arduino.h>
#include "hardware/AbstractHardware.h"

namespace MotorDrivers {
    class DShotMotor : public Hardware::Motor {
    public:
        void setOutput(int16_t output) override;
    };
}
