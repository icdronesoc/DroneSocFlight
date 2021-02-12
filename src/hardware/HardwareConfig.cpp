#include "HardwareConfig.h"

namespace HardwareConfig {
    namespace { // private
        // The variables we expect the target file to set
        extern const HardwareSerial UARTs[];

#ifdef BUILD_TARGET
#include STRINGIFY(BUILD_TARGET)
#else
#error No hardware config found.
#endif
    }

    const HardwareSerial* getUart(size_t uart) {
        if (uart >= uartCount) return nullptr;
        return &UARTs[uart];
    }

    const size_t uartCount = sizeof(UARTs) / sizeof(HardwareSerial);
}
