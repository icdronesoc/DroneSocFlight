#pragma once

#include <Arduino.h>
#include "rc/RC.h"
#include "rc/drivers/spi/CC2500.h"

namespace RcDrivers {
    class RedpineDriver : public RC::Driver {
    public:
        RedpineDriver(SPIClass &spi, uint32_t csPin);
        bool getFrame(RC::ChannelsMicroseconds &channels) override;
        bool shouldTryToGetFrame() override;

    private:
        CC2500::CC2500 cc2500;
    };
}
