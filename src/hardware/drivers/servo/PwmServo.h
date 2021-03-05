#pragma once

#include <Arduino.h>
#include "hardware/AbstractHardware.h"

#ifdef PLATFORM_ESP32
#include <ESP32Servo.h>
#else
#include <Servo.h> // TODO STM32 Servo library uses ISR-based driving...
#endif

namespace ServoDrivers {
    class PwmServo : public Hardware::Servo {
    public:
        PwmServo(uint32_t pin, uint32_t refreshRate);
        void setOutput(double output) override;
    private:
        ::Servo servo; // Without :: you get a conflict between the namespaced and global "Servo" classes
        uint32_t pin, refreshRate;
    };
}
