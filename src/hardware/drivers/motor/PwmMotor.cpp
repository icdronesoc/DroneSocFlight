#include "PwmMotor.h"

namespace MotorDrivers {
    void PwmMotor::initialize() {
#ifdef PLATFORM_ESP32
        this->servo.setPeriodHertz(50);
#endif
        this->servo.attach(this->pin);
    }

    void PwmMotor::setOutput(int16_t output) {
        this->servo.writeMicroseconds(map(output, 0, 2047, 1000, 2000));
    }
}
