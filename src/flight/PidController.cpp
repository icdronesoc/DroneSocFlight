#include "PidController.h"

#include <PID_v1.h>

namespace PidController {
    namespace { // private
        double pitchInput, pitchSetpoint, pitchOutput;
        PID pitchPid(&pitchInput, &pitchSetpoint, &pitchOutput, 0, 0, 0, P_ON_E, DIRECT);

        double rollInput, rollSetpoint, rollOutput;
        PID rollPid(&rollInput, &rollSetpoint, &rollOutput, 0, 0, 0, P_ON_E, DIRECT);

        double yawInput, yawSetpoint, yawOutput;
        PID yawPid(&yawInput, &yawSetpoint, &yawOutput, 0, 0, 0, P_ON_E, DIRECT);

        double pitchFeedForwardGain, rollFeedForwardGain, yawFeedForwardGain; // TODO implement FF controller
    }

    void setGains(Gains gains) {
        pitchPid.SetTunings(gains.pitch.P, gains.pitch.I, gains.pitch.D);
        rollPid.SetTunings(gains.roll.P, gains.roll.I, gains.roll.D);
        yawPid.SetTunings(gains.yaw.P, gains.yaw.I, gains.yaw.D);
        pitchFeedForwardGain = gains.pitch.FF;
        rollFeedForwardGain = gains.roll.FF;
        yawFeedForwardGain = gains.yaw.FF;
    }
}
