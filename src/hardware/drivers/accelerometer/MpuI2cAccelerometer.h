#pragma once

#include <Arduino.h>
#include "hardware/AbstractHardware.h"

namespace AccelerometerDrivers {
    class MpuI2cAccelerometer : public Hardware::Accelerometer {
    public:
        void initialize() override;
        Hardware::ThreeAxisData getAccelerationData() override;
    };
}
