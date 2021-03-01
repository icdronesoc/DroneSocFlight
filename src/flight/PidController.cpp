#include "PidController.h"

#include <PID_v1.h>

namespace PidController {
    Axis axisInputs;
    Axis axisSetpoints;
    Axis axisOutputs;
    int16_t throttleInput;
    int16_t throttleOutput;

    namespace { // private
        // TODO replace with our own PID controller
        // TODO set PID controller sample time
        PID pitchPid(&axisInputs.pitch, &axisSetpoints.pitch, &axisOutputs.pitch, 0, 0, 0, P_ON_E, DIRECT);

        PID rollPid(&axisInputs.roll, &axisSetpoints.roll, &axisOutputs.roll, 0, 0, 0, P_ON_E, DIRECT);

        PID yawPid(&axisInputs.yaw, &axisSetpoints.yaw, &axisOutputs.yaw, 0, 0, 0, P_ON_E, DIRECT);

        // TODO FF controller
    }

    void setGains(Gains gains) {
        pitchPid.SetTunings(gains.pitch.P, gains.pitch.I, gains.pitch.D);
        rollPid.SetTunings(gains.roll.P, gains.roll.I, gains.roll.D);
        yawPid.SetTunings(gains.yaw.P, gains.yaw.I, gains.yaw.D);
    }

    void compute() {
        // We don't do any throttle adjustments yet.
        throttleOutput = throttleInput;

        pitchPid.Compute();
        rollPid.Compute();
        yawPid.Compute();
    }
}
