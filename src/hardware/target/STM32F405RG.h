#include "_STM32_Common.h"

Pin pins[] = {
        // Taken from STM32 Arduino core
        {"PA0",  PA0},
        {"PA1",  PA1},
        {"PA2",  PA2},
        {"PA3",  PA3},
        {"PA4",  PA4},
        {"PA5",  PA5},
        {"PA6",  PA6},
        {"PA7",  PA7},
        {"PA8",  PA8},
        {"PA9",  PA9},
        {"PA10", PA10},
        {"PA11", PA11},
        {"PA12", PA12},
        {"PA13", PA13},
        {"PA14", PA14},
        {"PA15", PA15},
        {"PB0",  PB0},
        {"PB1",  PB1},
        {"PB2",  PB2},
        {"PB3",  PB3},
        {"PB4",  PB4},
        {"PB5",  PB5},
        {"PB6",  PB6},
        {"PB7",  PB7},
        {"PB8",  PB8},
        {"PB9",  PB9},
        {"PB10", PB10},
        {"PB11", PB11},
        {"PB12", PB12},
        {"PB13", PB13},
        {"PB14", PB14},
        {"PB15", PB15},
        {"PC0",  PC0},
        {"PC1",  PC1},
        {"PC2",  PC2},
        {"PC3",  PC3},
        {"PC4",  PC4},
        {"PC5",  PC5},
        {"PC6",  PC6},
        {"PC7",  PC7},
        {"PC8",  PC8},
        {"PC9",  PC9},
        {"PC10", PC10},
        {"PC11", PC11},
        {"PC12", PC12},
        {"PC13", PC13},
        {"PC14", PC14},
        {"PC15", PC15},
        {"PD2",  PD2},
        {"PH0",  PH0},
        {"PH1",  PH1},
};

void setupMcuHardware(IOConfig ioConfig) {
    // TODO use ioConfig
    UARTs.push_back(HardwareSerial(PA10, PA9));
    UARTs.push_back(HardwareSerial(PA3, PA2));
    UARTs.push_back(HardwareSerial(PC11, PC10));
    UARTs.push_back(HardwareSerial(PA1, PA0));
    UARTs.push_back(HardwareSerial(PD2, PC12));
    UARTs.push_back(HardwareSerial(PC7, PC6));

    SPIs.push_back(SPIClass(PA7, PA6, PA5));
    SPIs.push_back(SPIClass(PB15, PB14, PB13));
    SPIs.push_back(SPIClass(PC12, PC11, PC10));
    for (auto spi : SPIs) spi.begin();

    // Different FCs always define the first one or the second two but never any other combination...
    I2Cs.push_back(TwoWire(PB7, PB6));
    I2Cs.push_back(TwoWire(PB9, PB8));
    I2Cs.push_back(TwoWire(PB11, PB10));
    for (auto i2c : I2Cs) i2c.begin();
}
