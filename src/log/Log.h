#pragma once

#include <Arduino.h>

namespace Log {
    enum class Level {
        INFO,
        WARNING,
        ERROR,
    };

    void initialize();

    void info(const char* tag, const char* format, ...);
    void warning(const char* tag, const char* format, ...);
    void error(const char* tag, const char* format, ...);
}
