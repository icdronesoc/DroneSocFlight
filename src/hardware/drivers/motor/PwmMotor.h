#pragma once

#include <Arduino.h>
#include "hardware/AbstractHardware.h"
#include "hardware/pwm/PWM.h"

namespace MotorDrivers {
    class PwmMotor : public Hardware::Motor {
    public:
        explicit PwmMotor(PWM::Output& pwmTimer);
        void setOutput(double output) override;
    private:
        PWM::Output& pwmTimer;
    };
}
