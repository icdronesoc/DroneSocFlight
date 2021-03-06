#include "Config.h"
#include "log/Log.h"
#include "hardware/IO.h"
#include "pb_encode.h"
#include "pb_decode.h"

namespace Config {
    namespace {
        const char* LogTag = "Config";
        constexpr size_t BufferSize = 8 * 1024; // 8kB buffer
    }

    Configuration config = Configuration_init_default;

    void loadConfig() {
        uint8_t buffer[BufferSize];
        auto stream = pb_istream_from_buffer(buffer, BufferSize);

        if (IO::loadData(buffer, BufferSize) == 0) {
            Log::error(LogTag, "Error loading hardware configuration.");
            return;
        }

        if (!pb_decode(&stream, Configuration_fields, &config)) {
            Log::error(LogTag, "Error decoding hardware configuration: %s", stream.errmsg);
            return;
        }
    }

    void saveConfig() {
        uint8_t buffer[BufferSize];
        auto stream = pb_ostream_from_buffer(buffer, BufferSize);

        if (!pb_encode(&stream, Configuration_fields, &config)) {
            Log::error(LogTag, "Error encoding hardware configuration: %s", stream.errmsg);
            return;
        }

        if (!IO::storeData(buffer, stream.bytes_written)) {
            Log::error(LogTag, "Error storing hardware configuration.");
        }
    }
}
