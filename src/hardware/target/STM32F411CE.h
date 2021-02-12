#include "_STM32_Common.h"

HardwareSerial UARTs[] = {
        HardwareSerial(PA10, PA9),
        HardwareSerial(PA3, PA2),
};

SPIClass SPIs[] = {
        SPIClass(PA7, PA6, PA5),
        SPIClass(PB15, PB14, PB13),
};

TwoWire I2Cs[] = {
        // Different FCs always define one of these but never both...
        TwoWire(PB7, PB6),
        TwoWire(PB9, PB8),
};
