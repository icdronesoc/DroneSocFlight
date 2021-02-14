#include "_STM32_Common.h"

Pin pins[] = {
        // TODO
};

void setupMcuHardware(IOConfig ioConfig) {
    // TODO use ioConfig
    UARTs.push_back(HardwareSerial(PA10, PA9));
    UARTs.push_back(HardwareSerial(PA3, PA2));

    SPIs.push_back(SPIClass(PA7, PA6, PA5));
    SPIs.push_back(SPIClass(PB15, PB14, PB13));
    for (auto spi : SPIs) spi.begin();

    // Different FCs always define the first one or the second two but never any other combination...
    I2Cs.push_back(TwoWire(PB7, PB6));
    I2Cs.push_back(TwoWire(PB9, PB8));
    for (auto i2c : I2Cs) i2c.begin();
}
