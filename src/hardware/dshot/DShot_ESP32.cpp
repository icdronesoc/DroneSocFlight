#ifdef PLATFORM_ESP32

#include "DShot.h"
#include "log/Log.h"
#include "driver/rmt.h"

namespace DShot {
    namespace {
        const auto LogTag = "DShot Driver";

        constexpr uint8_t MaxRmtChannels = RMT_CHANNEL_MAX;
        uint8_t numberOfAllocateRmtChannels = 0;

        uint8_t allocateRmtChannel() {
            if (numberOfAllocateRmtChannels < MaxRmtChannels) {
                auto allocatedChannel = numberOfAllocateRmtChannels;
                numberOfAllocateRmtChannels++;
                return allocatedChannel;
            } else {
                Log::error(LogTag, "Cannot allocate RMT Channel: All channels are allocated.");
                return MaxRmtChannels;
            }
        }

        constexpr size_t DShotBitCount = 16;
        constexpr float RmtTickRate = 12.5; // nanoseconds per tick, this is the fastest the ESP32 RMT can go

        constexpr float DShot1200_BitPeriod = 1675; // nanoseconds
        constexpr float DShot1200_T0H = 312.5; // nanoseconds
        constexpr float DShot1200_T1H = 625; // nanoseconds

        class ESP32Driver : public Driver {
        public:
            ESP32Driver(rmt_config_t rmtConfig, uint32_t T0H, uint32_t T1H, uint32_t T0L, uint32_t T1L) : rmtConfig(rmtConfig), T0H(T0H), T1H(T1H), T0L(T0L), T1L(T1L) {}

            void sendPacket(uint16_t packet) override {
                this->setupForTx();

                rmt_item32_t pulses[DShotBitCount];
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

                rmt_write_items(rmtConfig.channel, pulses, DShotBitCount, false);

                // TODO once done, reconfigure as RX and wait for telemetry packet
//                rmt_register_tx_end_callback()
            }

        private:
            rmt_config_t rmtConfig;
            bool driverInstalled = false;
            uint32_t T0H, T1H, T0L, T1L;

            void setupForTx() {
                if (driverInstalled) rmt_driver_uninstall(rmtConfig.channel); // TODO error check
                rmtConfig.rmt_mode = RMT_MODE_TX;
                if (rmt_config(&this->rmtConfig) != ESP_OK) {
                    Log::error(LogTag, "RMT TX Configuration error");
                }
                rmt_driver_install(rmtConfig.channel, 0, 0);
                driverInstalled = true;
            }

            void setupForRx() {
                if (driverInstalled) rmt_driver_uninstall(rmtConfig.channel); // TODO error check
                rmtConfig.rmt_mode = RMT_MODE_RX;
                if (rmt_config(&this->rmtConfig) != ESP_OK) {
                    Log::error(LogTag, "RMT RX Configuration error");
                }
                rmt_driver_install(rmtConfig.channel, DShotBitCount /* TODO */, 0);
                driverInstalled = true;
            }
        };
    }

    Driver* createDriver(uint32_t pin, Speed speed) {
        auto allocatedChannel = allocateRmtChannel();
        if (allocatedChannel >= MaxRmtChannels) return nullptr;

        rmt_config_t config;
        memset(&config, 0, sizeof(rmt_config_t));
        config.channel = static_cast<rmt_channel_t>(allocatedChannel);
        config.clk_div = 1; // To get 12.5ns per tick
        config.gpio_num = static_cast<gpio_num_t>(pin);
        config.tx_config.idle_output_en = true;
        config.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;

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
        uint32_t T0H = DShot1200_T0H * multiplier / RmtTickRate;
        uint32_t T1H = DShot1200_T1H * multiplier / RmtTickRate;
        uint32_t T0L = (DShot1200_BitPeriod - DShot1200_T0H) * multiplier / RmtTickRate;
        uint32_t T1L = (DShot1200_BitPeriod - DShot1200_T1H) * multiplier / RmtTickRate;

        return new ESP32Driver(config, T0H, T1H, T0L, T1L);
    }
}

#endif
