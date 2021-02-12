#pragma once

#include <Arduino.h>

namespace PidController {
    typedef struct {
        double P;
        double I;
        double D;
        double FF;
    } AxisGains;

    typedef struct {
        AxisGains pitch;
        AxisGains roll;
        AxisGains yaw;
    } Gains;

    void setGains(Gains gains);
}
