#pragma once

#include <Arduino.h>

namespace Log {
    enum class Level {
        INFO,
        WARNING,
        ERROR,
    };

    constexpr size_t MAX_LOG_MESSAGE_SIZE = 128;

    extern bool infoEnabled;
    extern bool warningEnabled;
    extern bool errorEnabled;

    void initialize();

    /**
     * Master print function - all other helper functions eventually call this.
     * Prints to the log.
     * @param logLevel The log level string
     * @param tag The tag string
     * @param message The message string
     */
    void print(const char* logLevel, const char* tag, const char* message);

    template <typename ... Args>
    void print(const char* logLevel, const char* tag, const char* format, const Args&... args) {
        char buffer[MAX_LOG_MESSAGE_SIZE];
        snprintf(buffer, MAX_LOG_MESSAGE_SIZE, format, args...);
        print(logLevel, tag, buffer);
    }

    template <typename ... Args>
    void info(const char* tag, const char *format, const Args&... args) {
        if (infoEnabled) {
            print("INFO   ", tag, format, args...);
        }
    }

    template <typename ... Args>
    void warning(const char* tag, const char *format, const Args&... args) {
        if (warningEnabled) {
            print("WARNING", tag, format, args...);
        }
    }

    template <typename ... Args>
    void error(const char* tag, const char *format, const Args&... args) {
        if (errorEnabled) {
            print("ERROR  ", tag, format, args...);
        }
    }
}
