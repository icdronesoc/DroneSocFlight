#pragma once

#include <Arduino.h>

namespace Constants {
    extern const char* GIT_REVISION;

    constexpr uint32_t WatchDogTimeout = 1; // Seconds
}
