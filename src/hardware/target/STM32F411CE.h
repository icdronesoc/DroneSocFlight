#include "_STM32_Common.h"

const HardwareSerial UARTs[] = {
        HardwareSerial(PA10, PA9),
        HardwareSerial(PA3, PA2),
};

const SPIClass SPIs[] = {
        SPIClass(PA7, PA6, PA5),
        SPIClass(PB15, PB14, PB13),
};
