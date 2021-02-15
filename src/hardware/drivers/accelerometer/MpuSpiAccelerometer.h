#pragma once

#include <Arduino.h>
#include "hardware/AbstractHardware.h"

namespace AccelerometerDrivers {
    class MpuSpiAccelerometer : public Hardware::Accelerometer {
    public:
        void initialize() override;
        Hardware::ThreeAxisData getAccelerationData() override;
    };
}
