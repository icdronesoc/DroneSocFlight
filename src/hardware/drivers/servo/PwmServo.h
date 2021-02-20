#pragma once

#include <Arduino.h>
#include "hardware/AbstractHardware.h"

#ifdef PLATFORM_ESP32
#include <ESP32Servo.h>
#else
#include <Servo.h>
#endif

namespace ServoDrivers {
    class PwmServo : public Hardware::Servo {
    public:
        PwmServo(uint32_t pin, uint32_t refreshRate);
        void setOutput(int16_t output) override;
    private:
        ::Servo servo; // Without :: you get a conflict between the namespaced and global "Servo" classes
        uint32_t pin, refreshRate;
    };
}
