#include "Controllers.h"

#include "PidController.h"

namespace Controllers {
    Axis axisInputs;
    Axis axisSetpoints;
    Axis axisOutputs;
    double throttleInput;
    double throttleOutput;

    namespace { // private
        PidController pitchPid(axisInputs.pitch, axisSetpoints.pitch, axisOutputs.pitch);

        PidController rollPid(axisInputs.roll, axisSetpoints.roll, axisOutputs.roll);

        PidController yawPid(axisInputs.yaw, axisSetpoints.yaw, axisOutputs.yaw);
    }

//    void setGains(Gains gains) {
//        pitchPid.SetTunings(gains.pitch.P, gains.pitch.I, gains.pitch.D);
//        rollPid.SetTunings(gains.roll.P, gains.roll.I, gains.roll.D);
//        yawPid.SetTunings(gains.yaw.P, gains.yaw.I, gains.yaw.D);
//    }

    void compute() {
        // We don't do any throttle adjustments yet.
        throttleOutput = throttleInput;

        // TODO gains
        // TODO set period
        pitchPid.compute(0, 0, 0, 0);
        rollPid.compute(0, 0, 0, 0);
        yawPid.compute(0, 0, 0, 0);
    }
}
