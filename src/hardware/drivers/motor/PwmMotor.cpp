#include "PwmMotor.h"

namespace MotorDrivers {
    PwmMotor::PwmMotor(uint32_t pin) : pin(pin) {
#ifdef PLATFORM_ESP32
        this->servo.setPeriodHertz(50);
#endif
        this->servo.attach(this->pin);
    }

    void PwmMotor::setOutput(int16_t output) {
        this->servo.writeMicroseconds(map(output, 0, 2047, 1000, 2000));
    }
}
