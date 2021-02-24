#pragma once

#include <Arduino.h>

namespace PidController {
    struct AxisGains {
        double P;
        double I;
        double D;
        double FF;
    };

    struct Gains {
        AxisGains pitch;
        AxisGains roll;
        AxisGains yaw;
    };

    void setGains(Gains gains);
}
