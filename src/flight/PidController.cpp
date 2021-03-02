#include "PidController.h"

PidController::PidController(double& input, double& setpoint, double& output) : maxOutput(NAN), periodMicroseconds(1), input(input), setpoint(setpoint), output(output), lastSetpoint(0), lastError(0), lastIntegral(0) {}

void PidController::compute(double Kp, double Ki, double Kd, double Kff) {
    // Calculate error
    double error = this->setpoint - this->input;
    this->lastError = error;

    // Reset Sum
    this->output = 0;

    // Calculate Proportional
    if (Kp != 0) {
        this->output += Kp * error;
    }

    // Calculate Integral
    if (Ki != 0) {
        this->lastIntegral += error * this->periodMicroseconds;
        this->output += this->lastIntegral;
    }

    // Calculate Derivative
    if (Kd != 0) {
        double errorDelta = error - this->lastError;
        double derivative = errorDelta / this->periodMicroseconds;
        this->output += Kd * derivative;
    }

    // Calculate Feed Forward
    if (Kff != 0) {
        double setpointDelta = this->setpoint - this->lastSetpoint;
        double setpointDerivative = setpointDelta / this->periodMicroseconds;
        this->output += Kff * setpointDerivative;
        this->lastSetpoint = setpoint;
    }

    if (!isnan(this->maxOutput) && abs(this->output) > this->maxOutput) {
        if (this->output >= 0) {
            this->output = this->maxOutput;
        } else {
            this->output = -this->maxOutput;
        }
    }
}
