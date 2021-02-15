#include "IO.h"
#include <SoftwareSerial.h>

#ifdef PLATFORM_ESP32
#include <BluetoothSerial.h>
#endif

namespace IO {
    etl::vector<SerialPort*, maxNumberOfUARTs> UARTs;
    etl::vector<SPIClass*, maxNumberOfSPIs> SPIs;
    etl::vector<TwoWire*, maxNumberOfI2Cs> I2Cs;

    // What we expect the target file to define
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
        for (size_t i = 0; i < pinCount; i++) {
            if (strncmp(pinName, pins[i].name, maxPinNameLength) == 0) {
                return pins[i].number;
            }
        }
        return 0; // TODO handle error
    }

    void initialize() {
        setupMcuHardware(Config::hardwareConfig.ioConfig);
    }
}
