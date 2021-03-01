#include "PwmServo.h"
#include "utils/Maths.h"

namespace ServoDrivers {
    PwmServo::PwmServo(uint32_t pin, uint32_t refreshRate) : pin(pin), refreshRate(refreshRate) {
#ifdef PLATFORM_ESP32
        this->servo.setPeriodHertz(this->refreshRate);
#endif
        this->servo.attach(this->pin, 1000, 2000);
    }

    void PwmServo::setOutput(double output) {
        this->servo.write(static_cast<int>(Maths::map(output, -1, 1, 0, 180)));
    }
}
