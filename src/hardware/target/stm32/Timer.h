#pragma once
#ifdef PLATFORM_STM32

#include <Arduino.h>
#include "etl/map.h"

namespace Timer {
    constexpr size_t MaxTimerCount = 16;

    enum class Purpose {
        PWM,
        DShot,
    };

    struct TimerInfo {
        TimerInfo(TIM_TypeDef* timer, Purpose purpose) : timer(timer), purpose(purpose) {}
        TIM_TypeDef* timer;
        Purpose purpose;
    };

    extern etl::map<TIM_TypeDef*, TimerInfo*, MaxTimerCount> timers;

    /**
     * Manages allocation of timers.
     * Gets an already allocated timer or allocates a timer.
     * @param timerDescriptor The timer descriptor to get or allocate
     * @param purpose The purpose of the timer, to avoid collisions.
     * @return
     */
    TimerInfo *getOrAllocateTimer(uint32_t pin, Purpose purpose);
}

#endif
