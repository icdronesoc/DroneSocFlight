#include "AxisControllers.h"
#include "config/Config.h"
#include "debug/DebugInterface.h"
#include "PidController.h"

namespace AxisControllers {
    Axis axisInputs;
    Axis axisSetpoints;
    Axis axisOutputs;
    double throttleInput;
    double throttleOutput;

    namespace { // private
        // Gains are stored separately from the controller to allow for dynamic gain adjustment.

        PidGains pitchGains = PidGains(0, 0, 0, 0);
        PidController pitchPid(axisInputs.pitch, axisSetpoints.pitch, axisOutputs.pitch);

        PidGains rollGains = PidGains(0, 0, 0, 0);
        PidController rollPid(axisInputs.roll, axisSetpoints.roll, axisOutputs.roll);

        PidGains yawGains = PidGains(0, 0, 0, 0);
        PidController yawPid(axisInputs.yaw, axisSetpoints.yaw, axisOutputs.yaw);
    }

    void compute() {
        // We don't do any throttle adjustments yet.
        throttleOutput = throttleInput;

        // TODO set period
        pitchPid.compute(pitchGains);
        rollPid.compute(rollGains);
        yawPid.compute(yawGains);
    }

    void initialize() {
        if (Config::config.selectedPidProfile < Config::config.pidProfiles_count) {
            auto& profile = Config::config.pidProfiles[Config::config.selectedPidProfile];

            if (profile.has_pitch && profile.has_roll && profile.has_yaw) {
                pitchGains.Kp = profile.pitch.Kp;
                pitchGains.Ki = profile.pitch.Ki;
                pitchGains.Kd = profile.pitch.Kd;
                pitchGains.Kff = profile.pitch.Kff;
                rollGains.Kp = profile.roll.Kp;
                rollGains.Ki = profile.roll.Ki;
                rollGains.Kd = profile.roll.Kd;
                rollGains.Kff = profile.roll.Kff;
                yawGains.Kp = profile.yaw.Kp;
                yawGains.Ki = profile.yaw.Ki;
                yawGains.Kd = profile.yaw.Kd;
                yawGains.Kff = profile.yaw.Kff;
            } else {
                Debug::error("Selected PID profile does not contain gains for each axis.");
            }
        } else {
            Debug::error("Selected PID profile is out of range.");
        }
    }
}
