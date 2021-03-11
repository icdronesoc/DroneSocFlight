#pragma once

#include <Arduino.h>

namespace Maths {
    double map(double x, double in_min, double in_max, double out_min, double out_max);
    /**
     * Takes an input, rounds it up to the nearest power of 2 and returns the exponent.
     * @return The exponent of the next highest power of 2
     */
    uint8_t nextHighestPowerOf2(uint32_t input);
}
