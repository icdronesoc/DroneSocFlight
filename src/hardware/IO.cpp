#include "IO.h"
#include <SoftwareSerial.h>
#include "log/Log.h"

#if defined(PLATFORM_ESP32)
#include <BluetoothSerial.h>
//#include <esp_task_wdt.h>
#include <Preferences.h>
#elif defined(PLATFORM_STM32)
#include <IWatchdog.h>
#include <utils/CRC.h>
#endif

namespace IO {
    namespace { // private
        const auto LogTag = "IO";
    }

    constexpr uint32_t WatchDogTimeout = 1; // Seconds

    etl::map<UartName, SerialPort*, maxNumberOfUARTs> UARTs;
    etl::vector<SPIClass*, maxNumberOfSPIs> SPIs;
    etl::vector<TwoWire*, maxNumberOfI2Cs> I2Cs;

    // What we expect the target file to define
    void resetWatchdogTimer();
    extern Pin pins[];
    void setupMcuHardware();
    size_t loadData(byte *buffer, size_t maxSize);
    bool storeData(byte *data, size_t length);

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

    etl::vector<UartName, maxNumberOfUARTs> takenUARTs;

    SerialPort* takeUart(UartName uartName) {
        auto find = UARTs.find(uartName);
        if (find != UARTs.end()) {
            for (auto & takenUart : takenUARTs) {
                // Check if this UART is in the taken UARTs list
                if (takenUart == uartName) return nullptr;
            }
            // UART was not found in the taken UARTs list! Let's add it.
            takenUARTs.push_back(uartName);
            return find->second;
        } else {
            // UART does not exist.
            return nullptr;
        }
    }

    void initialize() {
        Log::info(LogTag, "Initializing IO");
        setupMcuHardware();
        Log::info(LogTag, "IO initialization complete.");
    }
}
