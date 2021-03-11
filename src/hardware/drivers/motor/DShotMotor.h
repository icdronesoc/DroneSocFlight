#pragma once

#include <Arduino.h>
#include "hardware/dshot/DShot.h"
#include "hardware/AbstractHardware.h"

namespace MotorDrivers {
    class DShotMotor : public Hardware::Motor {
    public:
        explicit DShotMotor(DShot::Output& dshotOutput);
        void setOutput(double output) override;

    private:
        DShot::Output& dShotOutput;
    };
}
