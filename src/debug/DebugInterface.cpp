#include "DebugInterface.h"
#include "config/Config.h"
#include "hardware/IO.h"
#include "etl/string.h"
#include "etl/to_string.h"

namespace Debug {
    namespace { // private
        constexpr size_t MAX_LOG_MESSAGE_SIZE = 128; // not including prefix

        IO::SerialPort* debugSerial = nullptr;
        etl::format_spec timestampFormat;
        bool infoEnabled = false;
        bool warningEnabled = false;
        bool errorEnabled = false;

        // debugLevel must be 7 chars long.
        void print(const char* debugLevel, const char* format, ...) {
            if (debugSerial != nullptr) {
                IO::SerialPort& output = *debugSerial;

                // 24 chars + null terminator
                etl::string<25> messagePrefix = "[";
                etl::to_string(millis(), messagePrefix, true);
                messagePrefix.append("] [");
                messagePrefix.append(debugLevel);
                messagePrefix.append("]: ");

                char buffer[MAX_LOG_MESSAGE_SIZE];
                va_list va;
                va_start(va, format);
                vsnprintf(buffer, MAX_LOG_MESSAGE_SIZE, format, va);
                va_end(va);

                output->write(messagePrefix.c_str(), messagePrefix.size());
                output->write(buffer);
                output->write("\r\n");
            }
        }
    }

    void initialize() {
        timestampFormat.decimal().width(10).fill(' ');

        if (Config::hardwareConfig.has_debugConfig) {
            infoEnabled = Config::hardwareConfig.debugConfig.infoEnabled;
            warningEnabled = Config::hardwareConfig.debugConfig.warningEnabled;
            errorEnabled = Config::hardwareConfig.debugConfig.errorEnabled;

            auto uart = IO::takeUart(Config::hardwareConfig.debugConfig.uartIndex);
            if (uart != nullptr) debugSerial = uart;
        }
    }

    void info(const char *format, ...) {
        if (infoEnabled) {
            va_list va;
            va_start(va, format);
            print("INFO   ", format, va);
            va_end(va);
        }
    }

    void warning(const char *format, ...) {
        if (warningEnabled) {
            va_list va;
            va_start(va, format);
            print("WARNING", format, va);
            va_end(va);
        }
    }

    void error(const char *format, ...) {
        if (errorEnabled) {
            va_list va;
            va_start(va, format);
            print("ERROR  ", format, va);
            va_end(va);
        }
    }
}
