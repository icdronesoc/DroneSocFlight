#pragma once

#include <Arduino.h>
#include "hardware/AbstractHardware.h"

namespace GyroscopeDrivers {
    class MpuSpiGyroscope : public Gyroscope {
    public:
        void initialize() override;
        ThreeAxisData getRotationData() override;
    };
}
