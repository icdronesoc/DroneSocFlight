#include "PwmServo.h"
#include "utils/Maths.h"

namespace ServoDrivers {
    PwmServo::PwmServo(PWM::Output& pwmTimer) : pwmTimer(pwmTimer) {
        this->pwmTimer.setPulseWidth(1500);
    }

    void PwmServo::setOutput(double output) {
        this->pwmTimer.setPulseWidth(Maths::map(output, -1, 1, 1000, 2000));
    }
}
