#include "PwmServo.h"

namespace ServoDrivers {
    PwmServo::PwmServo(uint32_t pin, uint32_t refreshRate) : pin(pin), refreshRate(refreshRate) {
#ifdef PLATFORM_ESP32
        this->servo.setPeriodHertz(this->refreshRate);
#endif
        this->servo.attach(this->pin, 1000, 2000);
    }

    void PwmServo::setOutput(int16_t output) {
        this->servo.write(map(output, -1024, 1023, 0, 180));
    }
}
