#include "IO.h"

#ifdef PLATFORM_ESP32
#include <BluetoothSerial.h>
#endif

namespace IO {
    etl::vector<HardwareSerial, maxNumberOfUARTs> UARTs;
    etl::vector<SPIClass, maxNumberOfSPIs> SPIs;
    etl::vector<TwoWire, maxNumberOfI2Cs> I2Cs;

    // What we expect the target file to define
    extern Stream* configUart;
    extern Pin pins[];
    void setupMcuHardware(IOConfig ioConfig);

    // Include target file
#ifdef BUILD_TARGET
#include STRINGIFY(BUILD_TARGET)
#else
#error No MCU hardware config found.
#endif

    const size_t pinCount = sizeof(pins) / sizeof(Pin);

    void initialize() {
        setupMcuHardware(Config::hardwareConfig.ioConfig);
    }
}
