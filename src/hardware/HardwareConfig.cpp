#include "HardwareConfig.h"

#define STM32F405RG 1
#define ESP32CAM 2
#define STM32F411CE 3

namespace HardwareConfig {
    namespace { // private
#if BUILD_TARGET == STM32F405RG
        const HardwareSerial UARTs[] = {
            HardwareSerial(PA10, PA9),
            HardwareSerial(PA3, PA2),
            HardwareSerial(PC11, PC10),
            HardwareSerial(PA1, PA0),
            HardwareSerial(PD2, PC12),
            HardwareSerial(PC7, PC6),
        };
#elif BUILD_TARGET == ESP32CAM
        const HardwareSerial UARTs[] = {
            Serial,
            Serial1,
            Serial2,
        };
#elif BUILD_TARGET == STM32F411CE
        const HardwareSerial UARTs[] = {
            HardwareSerial(PA10, PA9),
            HardwareSerial(PA3, PA2)
        };
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
