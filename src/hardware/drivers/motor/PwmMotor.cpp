#include "PwmMotor.h"
#include "utils/Maths.h"

namespace MotorDrivers {
    PwmMotor::PwmMotor(PWM::Output& pwmTimer) : pwmTimer(pwmTimer) {
        this->pwmTimer.setPulseWidth(1000);
    }

    void PwmMotor::setOutput(double output) {
        this->pwmTimer.setPulseWidth(Maths::map(output, 0, 1, 1000, 2000));
    }
}
