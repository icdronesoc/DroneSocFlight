#include "_STM32_Common.h"

const HardwareSerial UARTs[] = {
        HardwareSerial(PA10, PA9),
        HardwareSerial(PA3, PA2),
        HardwareSerial(PC11, PC10),
        HardwareSerial(PA1, PA0),
        HardwareSerial(PD2, PC12),
        HardwareSerial(PC7, PC6),
};

const SPIClass SPIs[] = {
        SPIClass(PA7, PA6, PA5),
        SPIClass(PB15, PB14, PB13),
        SPIClass(PC12, PC11, PC10),
};
