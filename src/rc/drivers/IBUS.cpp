#include "IBUS.h"

namespace RcDrivers {
    namespace { // private
        constexpr size_t ChannelCount = 14;
    }

    // TODO IBUS Telemetry
    IBUSDriver::IBUSDriver(IO::SerialPort& uart) : RC::Driver(ChannelCount) {
        // TODO calling begin() initializes the serial
//        this->ibus.begin(uart, IBUSBM_NOTIMER); TODO uart needs converting somehow
    }

    bool IBUSDriver::getFrame(RC::Channels& channels) {
        this->ibus.loop();
        if (this->ibus.cnt_rec - this->lastFrameCount > 0) {
            // We got a new frame!
            this->lastFrameCount = this->ibus.cnt_rec;
            for (size_t i = 0; i < ChannelCount; i++) {
                channels[i] = this->ibus.readChannel(i);
            }
            return true;
        } else {
            // No new frame.
            return false;
        }
    }
}
