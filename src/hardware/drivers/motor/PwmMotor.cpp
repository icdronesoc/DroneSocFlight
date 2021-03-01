#include "PwmMotor.h"
#include "utils/Maths.h"

namespace MotorDrivers {
    PwmMotor::PwmMotor(uint32_t pin) : pin(pin) {
#ifdef PLATFORM_ESP32 // TODO STM32 implementation
        this->servo.setPeriodHertz(50);
#endif
        this->servo.attach(this->pin);
    }

    void PwmMotor::setOutput(double output) {
        this->servo.writeMicroseconds(static_cast<int>(Maths::map(output, 0, 1, 1000, 2000)));
    }
}
