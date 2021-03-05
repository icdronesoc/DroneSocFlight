#include "FlightControlTasks.h"
#include "hardware/Hardware.h"
#include "scheduler/Scheduler.h"
#include "Controllers.h"
#include "Mixer.h"
#include "debug/DebugInterface.h"

namespace FlightControlTasks {
    namespace { // private
        const Scheduler::Name FlightControlScheduleName = "Flight Control";

        const Scheduler::Name GyroscopeTaskName = "Gyroscope";

        void readGyroscope() {
            // Hardware::gyroscope guaranteed not to be null because otherwise the task would never have been scheduled.
            auto gyroscopeData = Hardware::gyroscope->getRotationData();
            // TODO angle mode?
            // TODO check axis are correct
            Controllers::axisSetpoints.pitch = gyroscopeData.x;
            Controllers::axisSetpoints.roll = gyroscopeData.y;
            Controllers::axisSetpoints.yaw = gyroscopeData.z;
        }

        const Scheduler::Name AccelerometerTaskName = "Accelerometer";

        void readAccelerometer() {
            // Hardware::accelerometer guaranteed not to be null because otherwise the task would never have been scheduled.
            auto accelerometerData = Hardware::accelerometer->getAccelerationData();
            // TODO store
        }

        const Scheduler::Name PidTaskName = "PID Loop";

        void doPidTask() {
            Controllers::compute();
        }

        const Scheduler::Name MixerTaskName = "Mixer";

        void doMixerTask() {
            Mixer::applyMix(Controllers::throttleOutput, Controllers::axisOutputs.pitch, Controllers::axisOutputs.roll, Controllers::axisOutputs.yaw);
        }
    }

    void initialize() {
        if (Hardware::gyroscope == nullptr) {
            Debug::error("Cannot initialize Flight Control Tasks as no gyroscope is configured.");
            return;
        }

        if (Hardware::accelerometer != nullptr) {
            auto task = new Scheduler::Task(readAccelerometer, AccelerometerTaskName);
            auto schedule = new Scheduler::IndependentTaskSchedule(AccelerometerTaskName, task, Hardware::accelerometer->sampleRate);
            Scheduler::addTaskRunner(schedule);
            Debug::info("Accelerometer reading task configured.");
        }

        auto gyroTask = new Scheduler::Task(readGyroscope, GyroscopeTaskName);
        auto pidTask = new Scheduler::Task(doPidTask, PidTaskName);
        auto mixerTask = new Scheduler::Task(doMixerTask, MixerTaskName);
        uint8_t pidLoopFrequencyDivider = Config::hardwareConfig.pidLoopFrequencyDivider == 0 ? 1 : Config::hardwareConfig.pidLoopFrequencyDivider;
        auto sequenceSchedule = new Scheduler::SequentialTaskSchedule<3>(FlightControlScheduleName, {gyroTask, pidTask, mixerTask}, {pidLoopFrequencyDivider, 1}, Hardware::gyroscope->sampleRate);

        Scheduler::addTaskRunner(sequenceSchedule);
        Debug::info("Flight Control Tasks configured.");
    }
}
