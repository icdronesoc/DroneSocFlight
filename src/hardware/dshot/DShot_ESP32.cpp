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
        constexpr size_t DShotTelemetryBitCount = 20; // GCR encoding increases the bit count by 1.25x
        constexpr float RmtTickRate = 12.5; // nanoseconds per tick, this is the fastest the ESP32 RMT can go

        constexpr float DShot1200_BitPeriod = 833; // nanoseconds
        constexpr float DShot1200_T0H = 312.5; // nanoseconds
        constexpr float DShot1200_T1H = 625; // nanoseconds

        constexpr uint32_t T0H_TickCount = DShot1200_T0H / RmtTickRate;
        constexpr uint32_t T1H_TickCount = DShot1200_T1H / RmtTickRate;
        constexpr uint32_t T0L_TickCount = (DShot1200_BitPeriod - DShot1200_T0H) / RmtTickRate;
        constexpr uint32_t T1L_TickCount = (DShot1200_BitPeriod - DShot1200_T1H) / RmtTickRate;

        class StandardDriver : public Driver {
        public:
            explicit StandardDriver(rmt_config_t rmtConfig) : rmtConfig(rmtConfig) {
                if (rmt_config(&this->rmtConfig) != ESP_OK) {
                    Log::error(LogTag, "RMT TX Configuration error");
                    return;
                }
                if (rmt_driver_install(this->rmtConfig.channel, 0, 0) != ESP_OK) {
                    Log::error(LogTag, "RMT Driver Configuration error");
                    return;
                }
            }

            void sendPacket(uint16_t packet) override {
                rmt_item32_t pulses[DShotBitCount];
                // Iterate through bits, most-significant first.
                for (auto & pulse : pulses) {
                    pulse.level0 = 1;
                    pulse.level1 = 0;
                    if (packet & 0x8000) {
                        pulse.duration0 = T1H_TickCount;
                        pulse.duration1 = T1L_TickCount;
                    } else {
                        pulse.duration0 = T0H_TickCount;
                        pulse.duration1 = T0L_TickCount;
                    }
                    packet <<= 1;
                }

                if (rmt_write_items(rmtConfig.channel, pulses, DShotBitCount, false) != ESP_OK) {
                    Log::error(LogTag, "RMT write error");
                }
            }

        private:
            rmt_config_t rmtConfig;
        };

        /**
         * Many thanks to https://brushlesswhoop.com/dshot-and-bidirectional-dshot/#bidirectional-dshot
         */
        class BidirectionalTelemetryDriver : public Driver {
        public:
            BidirectionalTelemetryDriver(rmt_config_t txConfig, rmt_config_t rxConfig) : txConfig(txConfig), rxConfig(rxConfig) {
                if (rmt_driver_install(txConfig.channel, DShotTelemetryBitCount, 0) != ESP_OK) {
                    Log::error(LogTag, "RMT Driver Configuration error");
                    return;
                }
                // Configure for receive
                if (rmt_config(&this->rxConfig) != ESP_OK) {
                    Log::error(LogTag, "RMT RX Configuration error");
                    return;
                }
            }

            static void IRAM_ATTR onPacketSent(rmt_channel_t channel, void *arg) {
                auto rxConfig = reinterpret_cast<rmt_config_t*>(arg);
                digitalWrite(26, LOW);
                if (rmt_config(rxConfig) != ESP_OK) {
                    Log::error(LogTag, "RMT RX Configuration error"); // TODO this is unsafe in an ISR
                    return;
                }
                // TODO maybe we need to call rmt_rx_start
            }

            void sendPacket(uint16_t packet) override {
                // For bidirectional telemetry, we need to invert the CRC bits
                packet ^= 0xF;

                // TODO check for a telemetry packet
                RingbufHandle_t ringbuffer;
                if (rmt_get_ringbuf_handle(this->rxConfig.channel, &ringbuffer) != ESP_OK) {
                    Log::error(LogTag, "RMT write error");
                    return;
                }

                // Setup for TX
                if (rmt_config(&this->txConfig) != ESP_OK) {
                    Log::error(LogTag, "RMT TX Configuration error");
                    return;
                }

                rmt_item32_t pulses[DShotBitCount];
                // Iterate through bits, most-significant first.
                for (auto & pulse : pulses) {
                    pulse.level0 = 0;
                    pulse.level1 = 1;
                    if (packet & 0x8000) {
                        pulse.duration0 = T1H_TickCount;
                        pulse.duration1 = T1L_TickCount;
                    } else {
                        pulse.duration0 = T0H_TickCount;
                        pulse.duration1 = T0L_TickCount;
                    }
                    packet <<= 1;
                }

                digitalWrite(26, HIGH);
                if (rmt_write_items(txConfig.channel, pulses, DShotBitCount, false) != ESP_OK) {
                    Log::error(LogTag, "RMT write error");
                    return;
                }

                rmt_register_tx_end_callback(BidirectionalTelemetryDriver::onPacketSent, &this->rxConfig);
            }

        private:
            rmt_config_t txConfig, rxConfig;
        };
    }

    Driver* createDriver(uint32_t pin, Speed speed, bool bidirectionalTelemetry) {
        auto allocatedChannel = allocateRmtChannel();
        if (allocatedChannel >= MaxRmtChannels) return nullptr;
        Log::info(LogTag, "Allocated RMT channel %d", allocatedChannel);

        uint8_t rmtClockDivider = 8;
        switch (speed) {
            case Speed::DShot1200:
                rmtClockDivider = 1; // 12.5ns per tick
                break;
            case Speed::DShot600:
                rmtClockDivider = 2; // 25ns per tick
                break;
            case Speed::DShot300:
                rmtClockDivider = 4; // 50ns per tick
                break;
            case Speed::DShot150:
                rmtClockDivider = 8; // 100ns per tick
                break;
        }

        rmt_config_t txConfig = {
                .rmt_mode = RMT_MODE_TX,
                .channel = static_cast<rmt_channel_t>(allocatedChannel),
                .clk_div = rmtClockDivider,
                .gpio_num = static_cast<gpio_num_t>(pin),
                .mem_block_num = 1,
        };

        memset(&txConfig.tx_config, 0, sizeof(rmt_tx_config_t));
        txConfig.tx_config.idle_output_en = true;

        if (bidirectionalTelemetry) {
            txConfig.tx_config.idle_level = RMT_IDLE_LEVEL_HIGH;
            auto rxConfig = txConfig;
            rxConfig.rmt_mode = RMT_MODE_RX;
            memset(&rxConfig.rx_config, 0, sizeof(rmt_rx_config_t));
            return new BidirectionalTelemetryDriver(txConfig, rxConfig);
        } else {
            txConfig.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
            return new StandardDriver(txConfig);
        }
    }
}

#endif
