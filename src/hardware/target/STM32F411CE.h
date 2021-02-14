#include "_STM32_Common.h"

Pin pins[] = {
        // Taken from STM32 Arduino core
        {"PA00", PA0},
        {"PA01", PA1},
        {"PA02", PA2},
        {"PA03", PA3},
        {"PA04", PA4},
        {"PA05", PA5},
        {"PA06", PA6},
        {"PA07", PA7},
        {"PA08", PA8},
        {"PA09", PA9},
        {"PA10", PA10},
        {"PA11", PA11},
        {"PA12", PA12},
        {"PA13", PA13},
        {"PA14", PA14},
        {"PA15", PA15},
        {"PB00", PB0},
        {"PB01", PB1},
        {"PB02", PB2},
        {"PB03", PB3},
        {"PB04", PB4},
        {"PB05", PB5},
        {"PB06", PB6},
        {"PB07", PB7},
        {"PB08", PB8},
        {"PB09", PB9},
        {"PB10", PB10},
        {"PB12", PB12},
        {"PB13", PB13},
        {"PB14", PB14},
        {"PB15", PB15},
        {"PC13", PC13},
        {"PC14", PC14},
        {"PC15", PC15},
        {"PH00", PH0},
        {"PH01", PH1},
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
