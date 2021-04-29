#pragma once

#include <Arduino.h>
#include "config/Config.h"

namespace IO {
    constexpr size_t maxPinNameLength = sizeof(Pin::pinName) / sizeof(char);

    struct Pin {
        char name[maxPinNameLength];
        uint32_t number;
    };
}
