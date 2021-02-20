#include "IO.h"
#include <SoftwareSerial.h>

#if defined(PLATFORM_ESP32)
#include <BluetoothSerial.h>
#include <esp_task_wdt.h>
#elif defined(PLATFORM_STM32)
#include <IWatchdog.h>
#endif

namespace IO {
    const uint32_t WatchDogTimeout = 1; // Seconds

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

    const size_t pinCount = sizeof(pins) / sizeof(Pin);

    uint32_t pinNameToNumber(char* pinName) {
        for (auto & pin : pins) {
            if (strncmp(pinName, pin.name, maxPinNameLength) == 0) {
                return pin.number;
            }
        }
        return 0; // TODO handle error
    }

    void initialize() {
        setupMcuHardware(Config::hardwareConfig.ioConfig);
    }
}
