#pragma once

#include <Arduino.h>

struct PidGains {
    PidGains(double Kp, double Ki, double Kd, double Kff) : Kp(Kp), Ki(Ki), Kd(Kd), Kff(Kff) {}
    double Kp, Ki, Kd, Kff;
};

class PidController {
public:
    PidController(double &input, double &setpoint, double &output);

    void compute(PidGains& gains);

    /**
     * The output limit. The PID Output cannot be greater than +maxOutput or less than -maxOutput.
     * If this is set to NAN, there is no limit.
     * This must be a positive number.
     */
    double maxOutput;

    /**
     * The time period of the PID loop in microseconds.
     * Equal to 1 / frequency, where frequency is in MHz.
     * Defaults to 1us, so must be set as this is not an appropriate value.
     */
    uint32_t periodMicroseconds;

private:
    double &input;
    double &setpoint;
    double &output;
    double lastSetpoint;
    double lastError;
    double lastIntegral;
};
