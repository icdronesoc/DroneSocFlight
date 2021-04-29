#ifdef PLATFORM_STM32

#include "Timer.h"
#include "log/Log.h"

namespace Timer {
    namespace { // private
        constexpr auto LogTag = "Timer";
    }

    etl::map<TIM_TypeDef*, TimerInfo*, MaxTimerCount> timers;

    TimerInfo *getOrAllocateTimer(uint32_t pin, Purpose purpose) {
        auto timerDescriptor = reinterpret_cast<TIM_TypeDef*>(pinmap_peripheral(digitalPinToPinName(pin), PinMap_PWM));
        if (timerDescriptor == nullptr) {
            Log::error(LogTag, "No timer instance exists for pin %d", pin);
            return nullptr;
        }

        auto find = timers.find(timerDescriptor);
        if (find != timers.end()) {
            // Timer Info already exists (we're already using this timer)
            auto timerInfo = find->second;
            // Check for purpose collisions
            if (timerInfo->purpose != purpose) {
                Log::error(LogTag, "Cannot allocate timer because it is being used for a different purpose.");
                return nullptr;
            } else {
                return timerInfo;
            }
        } else {
            auto info = new TimerInfo(timerDescriptor, purpose);
            timers[timerDescriptor] = info;
            return info;
        }
    }
}

#endif
