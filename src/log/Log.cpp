#include "Log.h"
#include "config/Config.h"
#include "hardware/IO.h"
#include "etl/to_string.h"

namespace Log {
    namespace { // private
        constexpr size_t MAX_LOG_MESSAGE_SIZE = 128; // not including prefix

        IO::SerialPort* logSerial = nullptr;
        etl::format_spec timestampFormat;
        bool infoEnabled = false;
        bool warningEnabled = false;
        bool errorEnabled = false;

        // logLevel must be 7 chars long.
        void print(const char* logLevel, const char* tag, const char* format, ...) {
            if (logSerial != nullptr) {
                IO::SerialPort& output = *logSerial;

                etl::string<64> messagePrefix = "[";
                etl::to_string(millis(), messagePrefix, true);
                messagePrefix.append("] [");
                messagePrefix.append(logLevel);
                messagePrefix.append("] [");
                messagePrefix.append(tag);
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

        if (Config::config.has_logConfig) {
            infoEnabled = Config::config.logConfig.infoEnabled;
            warningEnabled = Config::config.logConfig.warningEnabled;
            errorEnabled = Config::config.logConfig.errorEnabled;

            auto uart = IO::takeUart(Config::config.logConfig.uartIndex);
            if (uart != nullptr) logSerial = uart;
        }
    }

    void info(const char* tag, const char *format, ...) {
        if (infoEnabled) {
            va_list va;
            va_start(va, format);
            print("INFO   ", tag, format, va);
            va_end(va);
        }
    }

    void warning(const char* tag, const char *format, ...) {
        if (warningEnabled) {
            va_list va;
            va_start(va, format);
            print("WARNING", tag, format, va);
            va_end(va);
        }
    }

    void error(const char* tag, const char *format, ...) {
        if (errorEnabled) {
            va_list va;
            va_start(va, format);
            print("ERROR  ", tag, format, va);
            va_end(va);
        }
    }
}
