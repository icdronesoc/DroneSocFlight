#include "Hardware.h"

#ifdef USE_ESP_BLUETOOTH_SERIAL
#include <BluetoothSerial.h>
BluetoothSerial bluetoothSerial;
#endif

namespace Hardware {
    // The variables we expect the target file to set
    extern const Stream* configurationUart;
    extern const HardwareSerial UARTs[];
    extern const SPIClass SPIs[];

    // Include target file
#ifdef BUILD_TARGET
#include STRINGIFY(BUILD_TARGET)
#else
#error No hardware config found.
#endif

    void initialize() {
#ifdef USE_ESP_BLUETOOTH_SERIAL
        bluetoothSerial.begin("DroneSoc FC");
#endif
    }

    constexpr size_t uartCount = sizeof(UARTs) / sizeof(HardwareSerial);
    const extern size_t spiCount = sizeof(SPIs) / sizeof(SPIClass);
}
