#ifdef PLATFORM_STM32

#include "Timer.h"
#include "log/Log.h"
#include "HardwareTimer.h"
#include "etl/map.h"

namespace Timer {
    namespace { // private
        const auto LogTag = "Timer";
        constexpr size_t MaxTimerCount = 16;

        enum class TimerPurpose {
            PWM,
        };

        struct TimerInfo {
            TimerInfo(HardwareTimer* timer, TimerPurpose purpose) : timer(timer), purpose(purpose) {}
            HardwareTimer* timer;
            TimerPurpose purpose;
        };

        etl::map<TIM_TypeDef*, TimerInfo*, MaxTimerCount> timers;
        etl::map<TIM_TypeDef*, uint32_t, MaxTimerCount> pwmFrequencies;

        /**
         * Manages allocation of timers.
         * Gets an already allocated timer or allocates a timer.
         * @param timerDescriptor The timer descriptor to get or allocate
         * @param purpose The purpose of the timer, to avoid collisions.
         * @return
         */
        TimerInfo* getOrAllocateTimer(TIM_TypeDef* timerDescriptor, TimerPurpose purpose) {
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
                // Create HardwareTimer (we're now using this timer)
                auto hardwareTimer = new HardwareTimer(timerDescriptor);
                auto info = new TimerInfo(hardwareTimer, purpose);
                timers[timerDescriptor] = info;
                return info;
            }
        }

        /**
         * Checks that setting up PWM on a timer channel will not overwrite an existing PWM frequency
         * @param timerDescriptor The timer descriptor for the timer
         * @param frequency The frequency of the PWM that we're about to setup
         * @return Whether there will be a collision and an existing frequency for another pin will be overwritten
         */
        bool checkForPwmFrequencyCollision(TIM_TypeDef* timerDescriptor, uint32_t frequency) {
            auto find = pwmFrequencies.find(timerDescriptor);
            if (find != pwmFrequencies.end()) {
                // This timer has already been setup for PWM - check that the frequencies are the same.
                auto existingFrequency = find->second;
                return frequency != existingFrequency;
            } else {
                // This timer has not been setup for PWM yet - no collision possible.
                pwmFrequencies[timerDescriptor] = frequency;
                return false;
            }
        }

        class STM32PWMTimer : public PWMTimer {
        public:
            STM32PWMTimer(HardwareTimer& hardwareTimer, uint32_t pinChannel) : hardwareTimer(hardwareTimer), pinChannel(pinChannel) {}

            void setDutyCycle(uint32_t pulseWidth) override {
                this->hardwareTimer.setCaptureCompare(this->pinChannel, pulseWidth, MICROSEC_COMPARE_FORMAT);
            }

        private:
            HardwareTimer& hardwareTimer;
            uint32_t pinChannel;
        };
    }

    PWMTimer* createPWMTimer(uint32_t pin, uint32_t frequency) {
        auto timerDescriptor = reinterpret_cast<TIM_TypeDef*>(pinmap_peripheral(digitalPinToPinName(pin), PinMap_PWM));
        if (timerDescriptor == nullptr) {
            Log::error(LogTag, "No timer instance exists for pin %d", pin);
            return nullptr;
        }

        auto timerInfo = getOrAllocateTimer(timerDescriptor, TimerPurpose::PWM);
        if (timerInfo == nullptr ||timerInfo->timer == nullptr) {
            Log::error(LogTag, "Could not get or create HardwareTimer");
            return nullptr;
        }

        if (checkForPwmFrequencyCollision(timerDescriptor, frequency)) {
            Log::error(LogTag, "Error setting PWM frequency: This timer's frequency has already been set to something else, and the older frequency would be overwritten.");
            return nullptr;
        }

        auto pinChannel = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(pin), PinMap_PWM));

        timerInfo->timer->setPWM(pinChannel, pin, frequency, 0);

        return new STM32PWMTimer(*timerInfo->timer, pinChannel);
    }
}

#endif
