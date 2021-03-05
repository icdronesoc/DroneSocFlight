#pragma once

#include <Arduino.h>

namespace Debug {
    enum class DebugLevel {
        INFO,
        WARNING,
        ERROR,
    };

    void initialize();

    void info(const char* format, ...);
    void warning(const char* format, ...);
    void error(const char* format, ...);
}
