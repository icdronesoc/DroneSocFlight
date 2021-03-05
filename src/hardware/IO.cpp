#include "IO.h"
#include <SoftwareSerial.h>
#include "debug/DebugInterface.h"

#if defined(PLATFORM_ESP32)
#include <BluetoothSerial.h>
#include <esp_task_wdt.h>
#elif defined(PLATFORM_STM32)
#include <IWatchdog.h>
#endif

namespace IO {
    constexpr uint32_t WatchDogTimeout = 1; // Seconds

    etl::vector<SerialPort*, maxNumberOfUARTs> UARTs;
    etl::vector<SPIClass*, maxNumberOfSPIs> SPIs;
    etl::vector<TwoWire*, maxNumberOfI2Cs> I2Cs;

    // What we expect the target file to define
    void resetWatchdogTimer();
    extern Pin pins[];
    void setupMcuHardware(IOConfig ioConfig);

    // Include target file
#ifdef BUILD_TARGET
#include STRINGIFY(BUILD_TARGET)
#else
#error No MCU hardware config found.
#endif

    constexpr size_t pinCount = sizeof(pins) / sizeof(Pin);

    Pin* findPin(char* pinName) {
        for (auto & pin : pins) {
            if (strncmp(pinName, pin.name, maxPinNameLength) == 0) {
                return &pin;
            }
        }
        return nullptr;
    }

    etl::vector<bool, maxNumberOfUARTs> uartIsTaken;

    SerialPort *takeUart(size_t uartIndex) {
        if (uartIndex >= UARTs.size()) return nullptr;
        auto uart = UARTs[uartIndex];
        if (uart == nullptr) return nullptr;
        if (uartIsTaken[uartIndex]) return nullptr;
        uartIsTaken[uartIndex] = true;
        return uart;
    }

    void initialize() {
        setupMcuHardware(Config::hardwareConfig.ioConfig);
        for (size_t i = 0; i < UARTs.size(); i++) uartIsTaken.push_back(false);
        Debug::info("IO Configured.");
    }
}
