#include "_STM32_Common.h"

HardwareSerial UARTs[] = {
        HardwareSerial(PA10, PA9),
        HardwareSerial(PA3, PA2),
        HardwareSerial(PC11, PC10),
        HardwareSerial(PA1, PA0),
        HardwareSerial(PD2, PC12),
        HardwareSerial(PC7, PC6),
};

SPIClass SPIs[] = {
        SPIClass(PA7, PA6, PA5),
        SPIClass(PB15, PB14, PB13),
        SPIClass(PC12, PC11, PC10),
};

TwoWire I2Cs[] = {
        // Different FCs always define the first one or the second two but never any other combination...
        TwoWire(PB7, PB6),
        TwoWire(PB9, PB8),
        TwoWire(PB11, PB10),
};
