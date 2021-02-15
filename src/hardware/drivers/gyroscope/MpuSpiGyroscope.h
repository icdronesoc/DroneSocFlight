#pragma once

#include <Arduino.h>
#include "hardware/AbstractHardware.h"

namespace GyroscopeDrivers {
    class MpuSpiGyroscope : public Hardware::Gyroscope {
    public:
        void initialize() override;
        Hardware::ThreeAxisData getRotationData() override;
    };
}
