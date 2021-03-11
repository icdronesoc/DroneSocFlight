#include "Log.h"
#include "config/Config.h"
#include "hardware/IO.h"
#include "etl/to_string.h"

namespace Log {
    namespace { // private
        const auto LogTag = "Log";
        IO::SerialPort* logSerial = nullptr;
        etl::format_spec timestampFormat;

        size_t longestTagLengthSeen = 0;
        size_t longestLogLevelLengthSeen = 0;
    }

    bool infoEnabled = true;
    bool warningEnabled = true;
    bool errorEnabled = true;

    void initialize() {
        timestampFormat.decimal().width(10).fill(' ');

        if (Config::config.has_logConfig && Config::config.logConfig.has_uart) {
            infoEnabled = Config::config.logConfig.infoEnabled;
            warningEnabled = Config::config.logConfig.warningEnabled;
            errorEnabled = Config::config.logConfig.errorEnabled;

            auto uart = IO::takeUart(Config::config.logConfig.uart.name);
            if (uart != nullptr) {
                logSerial = uart;
                logSerial->begin(Config::config.logConfig.baudRate);
                info(LogTag, "Logger initialized.");
            }
        }
    }

    void print(const char* logLevel, const char* tag, const char* message) {
        if (logSerial != nullptr) {
            IO::SerialPort& output = *logSerial;

            auto logLevelLength = strlen(logLevel);
            if (logLevelLength > longestLogLevelLengthSeen) longestLogLevelLengthSeen = logLevelLength;

            auto tagLength = strlen(tag);
            if (tagLength > longestTagLengthSeen) longestTagLengthSeen = tagLength;

            etl::string<64> messagePrefix = "[";
            etl::to_string(millis(), messagePrefix, timestampFormat, true);
            messagePrefix.append("] [");
            messagePrefix.append(logLevel);
            for (int i = 0; i < longestLogLevelLengthSeen - logLevelLength; i++) messagePrefix.append(" ");
            messagePrefix.append("] [");
            messagePrefix.append(tag);
            for (int i = 0; i < longestTagLengthSeen - tagLength; i++) messagePrefix.append(" ");
            messagePrefix.append("]: ");

            output->write(messagePrefix.c_str(), messagePrefix.size());
            output->write(message);
            output->write("\r\n");
        }
    }
}
