#ifdef PLATFORM_STM32

#include "PWM.h"
#include "log/Log.h"
#include "hardware/target/stm32/Timer.h"
#include "HardwareTimer.h"
#include "etl/map.h"

namespace PWM {
    namespace { // private
        const auto LogTag = "PWM";

        etl::map<TIM_TypeDef*, uint32_t, Timer::MaxTimerCount> pwmFrequencies;

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

        class STM32Output : public Output {
        public:
            STM32Output(HardwareTimer& hardwareTimer, uint32_t pinChannel) : hardwareTimer(hardwareTimer), pinChannel(pinChannel) {}

            void setPulseWidth(uint32_t pulseWidth) override {
                this->hardwareTimer.setCaptureCompare(this->pinChannel, pulseWidth, MICROSEC_COMPARE_FORMAT);
            }

        private:
            HardwareTimer& hardwareTimer;
            uint32_t pinChannel;
        };
    }

    Output* createOutput(uint32_t pin, uint32_t frequency) {
        auto timerInfo = Timer::getOrAllocateTimer(pin, Timer::Purpose::PWM);
        if (timerInfo == nullptr ||timerInfo->timer == nullptr) {
            Log::error(LogTag, "Could not get or create timer");
            return nullptr;
        }

        auto hardwareTimer = new HardwareTimer(timerInfo->timer);

        if (checkForPwmFrequencyCollision(timerInfo->timer, frequency)) {
            Log::error(LogTag, "Error setting PWM frequency: This timer's frequency has already been set to something else, and the older frequency would be overwritten.");
            return nullptr;
        }

        auto pinChannel = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(pin), PinMap_PWM));

        hardwareTimer->setPWM(pinChannel, pin, frequency, 0);

        return new STM32Output(*hardwareTimer, pinChannel);
    }
}

#endif
