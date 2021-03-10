#include "PwmServo.h"
#include "utils/Maths.h"

namespace ServoDrivers {
    PwmServo::PwmServo(Timer::PWMTimer& pwmTimer) : pwmTimer(pwmTimer) {
        this->pwmTimer.setDutyCycle(1500);
    }

    void PwmServo::setOutput(double output) {
        this->pwmTimer.setDutyCycle(Maths::map(output, -1, 1, 1000, 2000));
    }
}
