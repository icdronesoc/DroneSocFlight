#include "IO.h"
#include "log/Log.h"

namespace IO {
    namespace { // private
        const auto LogTag = "IO";
    }

    etl::map<UartName, SerialPort*, maxNumberOfUARTs> UARTs;
    etl::vector<SPIClass*, maxNumberOfSPIs> SPIs;
    etl::vector<TwoWire*, maxNumberOfI2Cs> I2Cs;

    const Pin* findPin(char* pinName) {
        for (size_t i = 0; i < Target::pinCount; i++) {
            if (strncmp(pinName, Target::pins[i].name, maxPinNameLength) == 0) {
                return &Target::pins[i];
            }
        }
        return nullptr;
    }

    etl::vector<UartName, maxNumberOfUARTs> takenUARTs;

    SerialPort* takeUart(const UartName& uartName) {
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
        Target::setupMcuHardware();
        Log::info(LogTag, "IO initialization complete.");
    }
}
