#pragma once

#include <Arduino.h>

/**
 * Manages the MCU's timer hardware and provides the application safe access to the timer hardware.
 * All features requiring timers should use this module as it can detect and report timer collisions.
 */
namespace Timer {
    class PWMTimer {
    public:
        /**
         * Sets the duty cycle of the PWM
         * @param dutyCycle The duty cycle, in microseconds
         */
        virtual void setDutyCycle(uint32_t dutyCycle) = 0;
    };

    /**
     * Gets a PWM timer for a specified pin.
     * This will setup the pin for PWM output.
     * @param pin The pin to output PWM on
     * @param frequency The frequency of the PWM output, in Hz
     * @return The PWM Timer, or nullptr if allocating or setting up the timer failed.
     */
    PWMTimer* getPWMTimerForOutputPin(uint32_t pin, uint32_t frequency);
}
