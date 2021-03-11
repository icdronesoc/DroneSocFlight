#ifdef PLATFORM_ESP32

#include "DShot.h"
#include "log/Log.h"
#include "esp32-hal-rmt.h"

namespace DShot {
    namespace {
        const auto LogTag = "DShot Driver";

        constexpr size_t DShotBitCount = 16;
        constexpr float RmtTickRate = 12.5; // nanoseconds per tick, this is the fastest the ESP32 RMT can go

        constexpr float DShot1200_BitPeriod = 1675; // nanoseconds
        constexpr float DShot1200_T0H = 312.5; // nanoseconds
        constexpr float DShot1200_T1H = 625; // nanoseconds

        class ESP32Driver : public Driver {
        public:
            ESP32Driver(rmt_obj_t* rmt, uint32_t T0H, uint32_t T1H, uint32_t T0L, uint32_t T1L) : rmt(rmt), T0H(T0H), T1H(T1H), T0L(T0L), T1L(T1L) {}

            void sendPacket(uint16_t packet) override {
                rmt_data_t pulses[DShotBitCount];
                // Iterate through bits, most-significant first.
                for (auto & pulse : pulses) {
                    pulse.level0 = 1;
                    pulse.level1 = 0;
                    if (packet & 0x8000) {
                        pulse.duration0 = this->T1H;
                        pulse.duration1 = this->T1L;
                    } else {
                        pulse.duration0 = this->T0H;
                        pulse.duration1 = this->T0L;
                    }
                    packet <<= 1;
                }
                if (!rmtWrite(rmt, pulses, DShotBitCount)) {
                    Log::error(LogTag, "Error performing RMT write");
                }
            }

        private:
            rmt_obj_t* rmt;
            uint32_t T0H, T1H, T0L, T1L;
        };
    }

    Driver* createDriver(uint32_t pin, Speed speed) {
        auto rmt = rmtInit(pin, true, RMT_MEM_128);
        if (rmt == nullptr) {
            Log::error(LogTag, "Error initializing RMT. Most likely cause is that all channels are in use.");
            return nullptr;
        }

        float realTickRate = rmtSetTick(rmt, RmtTickRate);
        if (realTickRate != RmtTickRate) {
            Log::warning(LogTag, "RMT Tick rate (%f) different to expected (%f)", realTickRate, RmtTickRate);
        }

        float multiplier = 1;
        switch (speed) {
            case Speed::DShot1200:
                multiplier = 1;
                break;
            case Speed::DShot600:
                multiplier = 2;
                break;
            case Speed::DShot300:
                multiplier = 4;
                break;
            case Speed::DShot150:
                multiplier = 8;
                break;
        }

        // Calculate number of timer ticks for each duration
        uint32_t T0H = DShot1200_T0H * multiplier / realTickRate;
        uint32_t T1H = DShot1200_T1H * multiplier / realTickRate;
        uint32_t T0L = (DShot1200_BitPeriod - DShot1200_T0H) * multiplier / realTickRate;
        uint32_t T1L = (DShot1200_BitPeriod - DShot1200_T1H) * multiplier / realTickRate;

        return new ESP32Driver(rmt, T0H, T1H, T0L, T1L);
    }
}

#endif
