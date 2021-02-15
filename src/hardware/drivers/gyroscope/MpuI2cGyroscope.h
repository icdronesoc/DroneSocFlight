#pragma once

#include <Arduino.h>
#include "hardware/AbstractHardware.h"

namespace GyroscopeDrivers {
    class MpuI2cGyroscope : public Hardware::Gyroscope {
    public:
        void initialize() override;
        Hardware::ThreeAxisData getRotationData() override;
    };
}
