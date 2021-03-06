#include "Config.h"
#include "debug/DebugInterface.h"
#include "hardware/IO.h"
#include "pb_encode.h"
#include "pb_decode.h"

namespace Config {
    namespace {
        constexpr size_t BufferSize = 8 * 1024; // 8kB buffer
    }

    Configuration config = Configuration_init_default;

    void loadConfig() {
        uint8_t buffer[BufferSize];
        auto stream = pb_istream_from_buffer(buffer, BufferSize);

        if (IO::loadData(buffer, BufferSize) == 0) {
            Debug::error("Error loading hardware configuration.");
            return;
        }

        if (!pb_decode(&stream, Configuration_fields, &config)) {
            Debug::error("Error decoding hardware configuration: %s", stream.errmsg);
            return;
        }
    }

    void saveConfig() {
        uint8_t buffer[BufferSize];
        auto stream = pb_ostream_from_buffer(buffer, BufferSize);

        if (!pb_encode(&stream, Configuration_fields, &config)) {
            Debug::error("Error encoding hardware configuration: %s", stream.errmsg);
            return;
        }

        if (!IO::storeData(buffer, stream.bytes_written)) {
            Debug::error("Error storing hardware configuration.");
        }
    }
}
