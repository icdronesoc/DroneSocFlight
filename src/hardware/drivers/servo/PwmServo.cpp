#include "PwmServo.h"

namespace ServoDrivers {
    void PwmServo::initialize() {
        this->servo.setPeriodHertz(this->refreshRate);
        this->servo.attach(this->pin, 1000, 2000);
    }

    void PwmServo::setOutput(int16_t output) {
        this->servo.write(map(output, -1024, 1023, 0, 180));
    }
}
