#include "Redpine.h"

namespace RcDrivers {
    namespace {
        constexpr size_t ChannelCount = 16;
    }

    RedpineDriver::RedpineDriver(SPIClass &spi, uint32_t csPin) : RC::Driver(ChannelCount), cc2500(CC2500::CC2500(spi, csPin)){}

    bool RedpineDriver::getFrame(RC::ChannelsMicroseconds &channels) {
        return false;
    }

    bool RedpineDriver::shouldTryToGetFrame() {
        return false;
    }
}
