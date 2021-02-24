#pragma once

#include <Arduino.h>
#include "IBusBM.h"
#include "rc/RC.h"
#include "hardware/AbstractSerialPort.h"

namespace RcDrivers {
    class IBUSDriver : public RC::Driver {
    public:
        explicit IBUSDriver(IO::SerialPort& uart);
        bool getFrame(RC::Channels& channels) override;

    private:
        IBusBM ibus;
        uint8_t lastFrameCount = 0;
    };
}