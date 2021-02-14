#pragma once

#include <Arduino.h>
#include "hardware/AbstractHardware.h"

namespace AccelerometerDrivers {
    class MpuSpiAccelerometer : public Accelerometer {
    public:
        void initialize() override;
        ThreeAxisData getAccelerationData() override;
    };
}
