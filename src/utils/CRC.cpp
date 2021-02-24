#include "CRC.h"

namespace CRC {
    uint8_t crc8_dvb_s2(uint8_t crc, uint8_t a) {
        crc ^= a;
        for (int i = 0; i < 8; i++) {
            crc <<= 1;
            if (crc & 0x80) crc ^= 0xD5;
        }
        return crc;
    }
}
