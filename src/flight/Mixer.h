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
    void applyMix(int16_t throttle, int16_t pitchPidOutput, int16_t rollPidOutput, int16_t yawPidOutput);
}
