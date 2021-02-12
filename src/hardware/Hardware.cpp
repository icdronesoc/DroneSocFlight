#include "Hardware.h"

#ifdef USE_ESP_BLUETOOTH_SERIAL
#include <BluetoothSerial.h>
BluetoothSerial bluetoothSerial;
#endif

namespace HardwareConfig {
    // The variables we expect the target file to set
    extern const Stream* configurationUart;
    extern const HardwareSerial UARTs[];

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

    const HardwareSerial* getUart(size_t uart) {
        if (uart >= uartCount) return nullptr;
        return &UARTs[uart];
    }

    constexpr size_t uartCount = sizeof(UARTs) / sizeof(HardwareSerial);
}
