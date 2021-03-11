#include "Log.h"
#include "config/Config.h"
#include "hardware/IO.h"
#include "etl/to_string.h"

namespace Log {
    namespace { // private
        IO::SerialPort* logSerial = nullptr;
        etl::format_spec timestampFormat;
    }

    bool infoEnabled = true;
    bool warningEnabled = true;
    bool errorEnabled = true;

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

    void print(const char* logLevel, const char* tag, const char* message) {
        if (logSerial != nullptr) {
            IO::SerialPort& output = *logSerial;

            etl::string<64> messagePrefix = "[";
            etl::to_string(millis(), messagePrefix, timestampFormat, true);
            messagePrefix.append("] [");
            messagePrefix.append(logLevel);
            messagePrefix.append("] [");
            messagePrefix.append(tag);
            messagePrefix.append("]: ");

            output->write(messagePrefix.c_str(), messagePrefix.size());
            output->write(message);
            output->write("\r\n");
        }
    }
}
