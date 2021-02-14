#pragma once

#include <Arduino.h>
#include "hardware/AbstractHardware.h"

namespace GyroscopeDrivers {
    class MpuI2cGyroscope : public Gyroscope {
    public:
        void initialize() override;
        ThreeAxisData getRotationData() override;
    };
}
