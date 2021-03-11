#include "Config.h"
#include "log/Log.h"
#include "hardware/IO.h"
#include "pb_encode.h"
#include "pb_decode.h"

namespace Config {
    namespace {
        const auto LogTag = "Config";
        constexpr size_t BufferSize = 8 * 1024; // 8kB buffer
        uint8_t configBuffer[BufferSize];

        Configuration defaultConfig() {
            Configuration config = Configuration_init_default;
#ifdef PLATFORM_ESP32
            config.has_ioConfig = true;
            config.ioConfig.uartConfigs_count = 1;
            config.ioConfig.uartConfigs[0].has_tx = true;
            config.ioConfig.uartConfigs[0].has_rx = true;
            strcpy(config.ioConfig.uartConfigs[0].name, "USB");
            strcpy(config.ioConfig.uartConfigs[0].rx.pinName, "USB_RX");
            strcpy(config.ioConfig.uartConfigs[0].tx.pinName, "USB_TX");
            config.has_logConfig = true;
            config.logConfig.uartIndex = 0;
            config.logConfig.baudRate = 115200;
            config.logConfig.infoEnabled = true;
            config.logConfig.warningEnabled = true;
            config.logConfig.errorEnabled = true;
#endif
            return config;
        }
    }

    Configuration config = Configuration_init_default;

    void loadConfig() {
        auto stream = pb_istream_from_buffer(configBuffer, BufferSize);

        if (IO::loadData(configBuffer, BufferSize) == 0) {
            Log::error(LogTag, "Error loading hardware configuration. Reverting to default.");
            config = defaultConfig();
            return;
        }

        if (!pb_decode(&stream, Configuration_fields, &config)) {
            Log::error(LogTag, "Error decoding hardware configuration: %s. Reverting to default.", stream.errmsg);
            config = defaultConfig();
            return;
        }
    }

    void saveConfig() {
        auto stream = pb_ostream_from_buffer(configBuffer, BufferSize);

        if (!pb_encode(&stream, Configuration_fields, &config)) {
            Log::error(LogTag, "Error encoding hardware configuration: %s", stream.errmsg);
            return;
        }

        if (!IO::storeData(configBuffer, stream.bytes_written)) {
            Log::error(LogTag, "Error storing hardware configuration.");
        }
    }
}
