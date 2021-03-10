#pragma once

#include <Arduino.h>
#include "hardware/AbstractHardware.h"
#include "hardware/Timer.h"

namespace ServoDrivers {
    class PwmServo : public Hardware::Servo {
    public:
        explicit PwmServo(Timer::PWMTimer& pwmTimer);
        void setOutput(double output) override;
    private:
        Timer::PWMTimer& pwmTimer;
    };
}
