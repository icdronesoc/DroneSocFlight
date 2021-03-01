#pragma once

#include <Arduino.h>
#include "config/Config.h"

namespace Mixer {
    constexpr size_t MaxMixerRuleCount = sizeof(MixerConfig::mixerRules) / sizeof(MixerRule);

    /**
     * Initializes the mixer.
     * Requires configuration to be loaded.
     */
    void initialize();
    void applyMix(double throttle, double pitchPidOutput, double rollPidOutput, double yawPidOutput);
}
