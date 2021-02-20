#pragma once

#include <Arduino.h>
#include "hardware/AbstractHardware.h"

#ifdef PLATFORM_ESP32
#include <ESP32Servo.h>
#else
#include <Servo.h>
#endif

namespace MotorDrivers {
    class PwmMotor : public Hardware::Motor {
    private:
        ::Servo servo; // Without :: you get a conflict between the namespaced and global "Servo" classes
        uint32_t pin;
    public:
        explicit PwmMotor(uint32_t pin);
        void setOutput(int16_t output) override;
    };
}
