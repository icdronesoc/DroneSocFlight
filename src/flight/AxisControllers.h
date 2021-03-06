#pragma once

#include <Arduino.h>

/**
 * The PID Controller comprises of the Axis controller which controls the craft's rate of rotation along each axis, and the throttle controller, which dynamically adjusts throttle based on what the Axis controller is doing.
 */
namespace AxisControllers {
    struct Axis {
        double pitch;
        double roll;
        double yaw;
    };

    /**
     * Axis controller inputs
     */
    extern Axis axisInputs;
    /**
     * Axis controller setpoints
     */
    extern Axis axisSetpoints;
    /**
     * Axis controller outputs
     */
    extern Axis axisOutputs;

    /**
     * Throttle controller input
     */
    extern double throttleInput;
    /**
     * Throttle controller output
     */
    extern double throttleOutput;

    /**
     * Initializes the AxisControllers. Requires configuration to be valid.
     */
    void initialize();

    /**
     * Runs PID controller computations.
     * Uses the values in axisInputs, axisSetpoints and throttleInput to compute the controller results and stores them in axisOutputs and throttleOutput.
     */
    void compute();
}
