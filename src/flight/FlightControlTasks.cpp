#include "FlightControlTasks.h"
#include "hardware/Hardware.h"
#include "scheduler/Scheduler.h"
#include "PidController.h"
#include "Mixer.h"

namespace FlightControlTasks {
    namespace { // private
        const Scheduler::Name FlightControlScheduleName = "Flight Control";

        const Scheduler::Name GyroscopeTaskName = "Gyroscope";

        void readGyroscope() {
            // Hardware::gyroscope guaranteed not to be null because otherwise the task would never have been scheduled.
            auto gyroscopeData = Hardware::gyroscope->getRotationData();
            // TODO angle mode?
            // TODO check axis are correct
            PidController::axisSetpoints.pitch = gyroscopeData.x;
            PidController::axisSetpoints.roll = gyroscopeData.y;
            PidController::axisSetpoints.yaw = gyroscopeData.z;
        }

        const Scheduler::Name AccelerometerTaskName = "Accelerometer";

        void readAccelerometer() {
            // Hardware::accelerometer guaranteed not to be null because otherwise the task would never have been scheduled.
            auto accelerometerData = Hardware::accelerometer->getAccelerationData();
            // TODO store
        }

        const Scheduler::Name PidTaskName = "PID Loop";

        void doPidTask() {
            PidController::compute();
        }

        const Scheduler::Name MixerTaskName = "Mixer";

        void doMixerTask() {
            Mixer::applyMix(PidController::throttleOutput, PidController::axisOutputs.pitch, PidController::axisOutputs.roll, PidController::axisOutputs.yaw);
        }
    }

    void initialize() {
        if (Hardware::gyroscope == nullptr) {
            // TODO produce error message
            return;
        }

        if (Hardware::accelerometer != nullptr) {
            auto task = new Scheduler::Task(readAccelerometer, AccelerometerTaskName);
            auto schedule = new Scheduler::IndependentTaskSchedule(AccelerometerTaskName, task, Hardware::accelerometer->sampleRate);
            Scheduler::addTaskRunner(schedule);
        }

        auto gyroTask = new Scheduler::Task(readGyroscope, GyroscopeTaskName);
        auto pidTask = new Scheduler::Task(doPidTask, PidTaskName);
        auto mixerTask = new Scheduler::Task(doMixerTask, MixerTaskName);
        auto pidLoopFrequencyDivider = Config::hardwareConfig.pidLoopFrequencyDivider == 0 ? 1 : Config::hardwareConfig.pidLoopFrequencyDivider;
        auto sequenceSchedule = new Scheduler::SequentialTaskSchedule<3>(FlightControlScheduleName, new Scheduler::Task*[3]{gyroTask, pidTask, mixerTask}, new uint8_t[2]{static_cast<uint8_t>(pidLoopFrequencyDivider), 1}, Hardware::gyroscope->sampleRate);

        Scheduler::addTaskRunner(sequenceSchedule);
    }
}
