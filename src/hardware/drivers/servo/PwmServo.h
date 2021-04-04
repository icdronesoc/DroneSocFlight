#pragma once

#include <Arduino.h>
#include "hardware/AbstractHardware.h"
#include "hardware/pwm/PWM.h"

namespace ServoDrivers {
    class PwmServo : public Hardware::Servo {
    public:
        explicit PwmServo(PWM::Output& pwmTimer);
        void setOutput(double output) override;
    private:
        PWM::Output& pwmTimer;
    };
}
