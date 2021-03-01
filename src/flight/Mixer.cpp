#include "Mixer.h"
#include "etl/vector.h"
#include "hardware/Hardware.h"

namespace Mixer {
    namespace { // private
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
            float weight;
        };

        int16_t *motorOutputBuffer;
        size_t motorOutputBufferSize;
        int16_t *servoOutputBuffer;
        size_t servoOutputBufferSize;

        etl::vector<ValidatedMixerRule, MaxMixerRuleCount> mixerRules;
    }

    void initialize() {
        motorOutputBufferSize = Hardware::motors.size();
        motorOutputBuffer = new int16_t[motorOutputBufferSize];
        servoOutputBufferSize = Hardware::servos.size();
        servoOutputBuffer = new int16_t[servoOutputBufferSize];

        // Validate mixer rules TODO produce error when validation fails
        for (pb_size_t i = 0; i < Config::hardwareConfig.mixerConfig.mixerRules_count; i++) {
            // Check weight
            if (Config::hardwareConfig.mixerConfig.mixerRules[i].weight < -1 ||
                Config::hardwareConfig.mixerConfig.mixerRules[i].weight > 1)
                continue;

            // Check source
            MixerSource source;
            switch (Config::hardwareConfig.mixerConfig.mixerRules[i].source) {
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
                    continue;
            }

            // Check target
            auto targetIndex = Config::hardwareConfig.mixerConfig.mixerRules[i].targetIndex;
            TargetType targetType;
            switch (Config::hardwareConfig.mixerConfig.mixerRules[i].targetType) {
                case MixerRule_TargetType_MOTOR:
                    if (Hardware::motors.size() <= targetIndex || Hardware::motors[targetIndex] == nullptr) continue;
                    targetType = TargetType::MOTOR;
                    break;
                case MixerRule_TargetType_SERVO:
                    if (Hardware::servos.size() <= targetIndex || Hardware::servos[targetIndex] == nullptr) continue;
                    targetType = TargetType::SERVO;
                    break;
                default:
                    continue;
            }

            // Add validated rule
            mixerRules.push_back(ValidatedMixerRule{
                    .targetType = targetType,
                    .targetIndex = targetIndex,
                    .source = source,
                    .weight = Config::hardwareConfig.mixerConfig.mixerRules[i].weight
            });
        }
    }

    void applyMix(int16_t throttle, double pitchPidOutput, double rollPidOutput, double yawPidOutput) {
        memset(motorOutputBuffer, 0, motorOutputBufferSize * sizeof(int16_t));
        memset(servoOutputBuffer, 0, motorOutputBufferSize * sizeof(int16_t));

        for (auto &mixerRule : mixerRules) {
            float selectedInput;
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
            }

            int16_t** buffer;
            switch (mixerRule.targetType) {
            case TargetType::MOTOR:
                buffer = &motorOutputBuffer;
                break;
            case TargetType::SERVO:
                buffer = &servoOutputBuffer;
                break;
            }

            buffer[mixerRule.targetIndex] += static_cast<int16_t>(mixerRule.weight * selectedInput);
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
