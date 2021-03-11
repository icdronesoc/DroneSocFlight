#include "Mixer.h"
#include "etl/vector.h"
#include "hardware/Hardware.h"
#include "log/Log.h"

namespace Mixer {
    namespace { // private
        const auto LogTag = "Mixer";

        enum class MixerSource {
            THROTTLE,
            PITCH,
            ROLL,
            YAW,
        };

        enum class TargetType {
            MOTOR,
            SERVO,
        };

        struct ValidatedMixerRule {
            TargetType targetType;
            size_t targetIndex;
            MixerSource source;
            double weight;
        };

        double *motorOutputBuffer;
        size_t motorOutputBufferSize;
        double *servoOutputBuffer;
        size_t servoOutputBufferSize;

        etl::vector<ValidatedMixerRule, MaxMixerRuleCount> mixerRules;
    }

    void initialize() {
        motorOutputBufferSize = Hardware::motors.size();
        motorOutputBuffer = new double[motorOutputBufferSize];
        servoOutputBufferSize = Hardware::servos.size();
        servoOutputBuffer = new double[servoOutputBufferSize];

        // Validate mixer rules
        if (Config::config.has_mixerConfig) {
            for (pb_size_t i = 0; i < Config::config.mixerConfig.mixerRules_count; i++) {
                // Check weight
                if (Config::config.mixerConfig.mixerRules[i].weight < -1 ||
                    Config::config.mixerConfig.mixerRules[i].weight > 1) {
                    Log::error(LogTag, "Mixer rule %d weight out of range.", i);
                    continue;
                }

                // Check source
                MixerSource source;
                switch (Config::config.mixerConfig.mixerRules[i].source) {
                    case MixerRule_Source_THROTTLE:
                        source = MixerSource::THROTTLE;
                        break;
                    case MixerRule_Source_PITCH:
                        source = MixerSource::PITCH;
                        break;
                    case MixerRule_Source_ROLL:
                        source = MixerSource::ROLL;
                        break;
                    case MixerRule_Source_YAW:
                        source = MixerSource::YAW;
                        break;
                    default:
                        Log::error(LogTag, "Mixer rule %d source invalid.", i);
                        continue;
                }

                // Check target
                auto targetIndex = Config::config.mixerConfig.mixerRules[i].targetIndex;
                TargetType targetType;
                switch (Config::config.mixerConfig.mixerRules[i].targetType) {
                    case MixerRule_TargetType_MOTOR:
                        if (Hardware::motors.size() <= targetIndex || Hardware::motors[targetIndex] == nullptr) {
                            Log::error(LogTag, "Mixer rule %d: cannot find motor %d.", i, targetIndex);
                            continue;
                        }
                        targetType = TargetType::MOTOR;
                        break;
                    case MixerRule_TargetType_SERVO:
                        if (Hardware::servos.size() <= targetIndex || Hardware::servos[targetIndex] == nullptr) {
                            Log::error(LogTag, "Mixer rule %d: cannot find servo %d.", i, targetIndex);
                            continue;
                        }
                        targetType = TargetType::SERVO;
                        break;
                    default:
                        Log::error(LogTag, "Mixer rule %d: target type invalid.", i);
                        continue;
                }

                // Add validated rule
                mixerRules.push_back(ValidatedMixerRule{
                        .targetType = targetType,
                        .targetIndex = targetIndex,
                        .source = source,
                        .weight = Config::config.mixerConfig.mixerRules[i].weight
                });
            }
        } else {
            Log::error(LogTag, "Mixer not configured.");
        }
    }

    void applyMix(double throttle, double pitchPidOutput, double rollPidOutput, double yawPidOutput) {
        memset(motorOutputBuffer, 0, motorOutputBufferSize * sizeof(double));
        memset(servoOutputBuffer, 0, motorOutputBufferSize * sizeof(double));

        for (auto &mixerRule : mixerRules) {
            double selectedInput;
            switch (mixerRule.source) {
                case MixerSource::THROTTLE:
                    selectedInput = throttle;
                    break;
                case MixerSource::PITCH:
                    selectedInput = pitchPidOutput;
                    break;
                case MixerSource::ROLL:
                    selectedInput = rollPidOutput;
                    break;
                case MixerSource::YAW:
                    selectedInput = yawPidOutput;
                    break;
                default:
                    // Should be impossible as mixer rules are validated in initialization
                    Log::error(LogTag, "Mixer rule source invalid.");
                    continue;
            }

            double** buffer;
            switch (mixerRule.targetType) {
            case TargetType::MOTOR:
                buffer = &motorOutputBuffer;
                break;
            case TargetType::SERVO:
                buffer = &servoOutputBuffer;
                break;
            default:
                Log::error(LogTag, "Mixer rule target type invalid.");
                continue;
            }

            (*buffer)[mixerRule.targetIndex] += mixerRule.weight * selectedInput;
        }

        for (size_t i = 0; i < motorOutputBufferSize; i++) {
            // TODO constrain min/max
            Hardware::motors[i]->setOutput(motorOutputBuffer[i]);
        }
        for (size_t i = 0; i < servoOutputBufferSize; i++) {
            // TODO constrain min/max
            Hardware::servos[i]->setOutput(servoOutputBuffer[i]);
        }
    }
}
