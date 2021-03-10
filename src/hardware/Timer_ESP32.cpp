#ifdef PLATFORM_ESP32

#include "Timer.h"
#include "esp32-hal-timer.h"

namespace Timer {
    namespace { // private
        const char *LogTag = "Timer";
    }

    PWMTimer* getPWMTimerForOutputPin(uint32_t pin, uint32_t frequency) {

    }
}

#endif
