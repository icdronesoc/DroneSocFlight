#pragma once

#include <Arduino.h>
#include "hardware/AbstractHardware.h"

namespace AccelerometerDrivers {
    class MpuI2cAccelerometer : public Accelerometer {
    public:
        void initialize() override;
        ThreeAxisData getAccelerationData() override;
    };
}
