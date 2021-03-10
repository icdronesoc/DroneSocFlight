#include "PwmMotor.h"
#include "utils/Maths.h"

namespace MotorDrivers {
    PwmMotor::PwmMotor(Timer::PWMTimer& pwmTimer) : pwmTimer(pwmTimer) {
        this->pwmTimer.setDutyCycle(1000);
    }

    void PwmMotor::setOutput(double output) {
        this->pwmTimer.setDutyCycle(Maths::map(output, 0, 1, 1000, 2000));
    }
}
