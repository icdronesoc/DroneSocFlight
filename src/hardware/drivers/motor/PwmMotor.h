#pragma once

#include <Arduino.h>
#include "hardware/AbstractHardware.h"
#include "hardware/Timer.h"

namespace MotorDrivers {
    class PwmMotor : public Hardware::Motor {
    public:
        explicit PwmMotor(Timer::PWMTimer& pwmTimer);
        void setOutput(double output) override;
    private:
        Timer::PWMTimer& pwmTimer;
    };
}
