#pragma once

#include <Arduino.h>

namespace CRC {
    uint8_t crc8_dvb_s2(uint8_t crc, uint8_t a);
}
