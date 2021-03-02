#pragma once

#include <Arduino.h>

/**
 * CRCUtils provides CRC utility functions.
 *
 * CRC is a global macro on STM32 so we can't name this module CRC.
 */
namespace CRCUtils {
    uint8_t crc8_dvb_s2(uint8_t crc, uint8_t a);
}
