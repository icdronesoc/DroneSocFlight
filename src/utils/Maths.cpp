#include "Maths.h"

namespace Maths {
    double map(double x, double in_min, double in_max, double out_min, double out_max) {
        double divisor = (in_max - in_min);
        if(divisor == 0) return NAN;
        return (x - in_min) * (out_max - out_min) / divisor + out_min;
    }

    uint8_t nextHighestPowerOf2(uint32_t input) {
        return ceil(log(input)/log(2));
    }
}
