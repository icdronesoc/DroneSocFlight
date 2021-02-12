#pragma once

#include <Arduino.h>

/**
 * There are two types of output: Motor and Servo.
 * All output values are stored as int16_t types, and have 11 bit ranges.
 * Servo output values are signed (-1024 to 1023) and motor output values are unsigned (0 to 2047)
 */
namespace Outputs {
    void loopTask();
}
