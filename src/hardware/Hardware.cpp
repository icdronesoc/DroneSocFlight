#include "Hardware.h"

#ifdef USE_ESP_BLUETOOTH_SERIAL
#include <BluetoothSerial.h>
BluetoothSerial bluetoothSerial;
#endif

namespace Hardware {
    // The variables we expect the target file to set
    extern Stream* configurationUart;
    extern HardwareSerial UARTs[];
    extern SPIClass SPIs[];
    extern TwoWire I2Cs[];

    // Include target file
#ifdef BUILD_TARGET
#include STRINGIFY(BUILD_TARGET)
#else
#error No hardware config found.
#endif

    size_t uartCount = sizeof(UARTs) / sizeof(HardwareSerial);
    size_t spiCount = sizeof(SPIs) / sizeof(SPIClass);
    size_t i2cCount = sizeof(I2Cs) / sizeof(TwoWire);

    void initialize() {
#ifdef USE_ESP_BLUETOOTH_SERIAL
        bluetoothSerial.begin("DroneSoc FC");
#endif
        // Don't initialize other UARTs here as we don't know their baud rate yet

        // Initialize SPI buses
        for (int i = 0; i < spiCount; i++) {
            SPIs[i].begin();
        }

        // Initialize I2C buses
        for (int i = 0; i < i2cCount; i++) {
            I2Cs[i].begin();
        }
    }
}
