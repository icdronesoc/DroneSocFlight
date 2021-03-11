#ifdef PLATFORM_ESP32

#include "Timer.h"
#include "log/Log.h"
#include "esp32-hal-ledc.h"
#include "etl/vector.h"
#include "utils/Maths.h"

namespace Timer {
    namespace { // private
        const auto LogTag = "Timer";
        constexpr size_t MaxPWMCount = 16;

        uint8_t numberOfAllocatedPWMChannels = 0;
        /**
         * Every 2 PWM channels shares a timer, so must share the same frequency.
         */
        etl::vector<uint32_t, MaxPWMCount / 2> pwmTimerFrequencies;

        uint8_t allocatePWMChannel() {
            if (numberOfAllocatedPWMChannels < MaxPWMCount) {
                auto allocatedChannel = numberOfAllocatedPWMChannels;
                if (allocatedChannel == 8) Log::warning(LogTag, "All high-speed PWM channels have been allocated.");
                numberOfAllocatedPWMChannels++;
                return allocatedChannel;
            } else {
                Log::error(LogTag, "Cannot allocate PWM Channel: All channels are allocated.");
                return MaxPWMCount;
            }
        }

        /**
         * Checks that setting up PWM on a channel will not overwrite an existing PWM frequency
         * @return Whether there will be a collision and an existing frequency for another pin will be overwritten
         */
        bool checkForPwmFrequencyCollision(uint8_t pwmChannel, uint32_t frequency) {
            uint8_t timerNumber = pwmChannel / 2;
            if (timerNumber < pwmTimerFrequencies.size()) {
                // This timer has already had a frequency allocated.
                auto existingFrequency = pwmTimerFrequencies[timerNumber];
                return existingFrequency != frequency;
            } else {
                // This timer has not had a frequency allocated yet.
                pwmTimerFrequencies.push_back(frequency);
                return false;
            }
        }

        class ESP32PWMTimer : public PWMTimer {
        public:
            ESP32PWMTimer(uint8_t pwmChannel, double tickRate) : pwmChannel(pwmChannel),  tickRate(tickRate) {}

            void setPulseWidth(uint32_t pulseWidth) override {
                ledcWrite(pwmChannel, pulseWidth / this->tickRate);
            }

        private:
            uint32_t pwmChannel;
            double tickRate; // In microseconds per tick
        };
    }

    PWMTimer* createPWMTimer(uint32_t pin, uint32_t frequency) {
        auto pwmChannel = allocatePWMChannel();
        if (pwmChannel >= MaxPWMCount) return nullptr;

        if (checkForPwmFrequencyCollision(pwmChannel, frequency)) {
            Log::error(LogTag, "Error setting PWM frequency: This timer's frequency has already been set to something else, and the older frequency would be overwritten.");
            return nullptr;
        }

        // Period (in microseconds) = 1000000 / frequency (in Hz)
        auto periodMicroseconds = 1000000 / frequency;
        // Resolution must be >= period in microseconds to achieve >= 1 tick per microseconds
        auto resolutionBits = Maths::nextHighestPowerOf2(periodMicroseconds);
        // Tick Rate (in microseconds per tick) = period (in microseconds) / resolution (in ticks)
        auto tickRate = static_cast<double>(periodMicroseconds) / static_cast<double>(1 << resolutionBits);

        ledcSetup(pwmChannel, frequency, resolutionBits);
        ledcAttachPin(pin, pwmChannel);

        return new ESP32PWMTimer(pwmChannel, tickRate);
    }
}

#endif
