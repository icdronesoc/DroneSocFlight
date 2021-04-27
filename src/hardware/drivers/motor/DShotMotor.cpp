#include "DShotMotor.h"

namespace MotorDrivers {
    DShotMotor::DShotMotor(DShot::Output& dshotOutput) : dShotOutput(dshotOutput) {
        dshotOutput.writeThrottleValue(0);
    }

    void DShotMotor::setOutput(double output) {
        this->dShotOutput.writeThrottleValue(output);
    }
}
