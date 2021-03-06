#include "FlightControlTasks.h"
#include "hardware/Hardware.h"
#include "scheduler/Scheduler.h"
#include "AxisControllers.h"
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
            // TODO downscale if PID is running at a lower speed
            AxisControllers::axisSetpoints.pitch = gyroscopeData.x;
            AxisControllers::axisSetpoints.roll = gyroscopeData.y;
            AxisControllers::axisSetpoints.yaw = gyroscopeData.z;
        }

        const Scheduler::Name AccelerometerTaskName = "Accelerometer";

        void readAccelerometer() {
            // Hardware::accelerometer guaranteed not to be null because otherwise the task would never have been scheduled.
            auto accelerometerData = Hardware::accelerometer->getAccelerationData();
            // TODO store
        }

        const Scheduler::Name PidTaskName = "PID Loop";

        void doPidTask() {
            AxisControllers::compute();
        }

        const Scheduler::Name MixerTaskName = "Mixer";

        void doMixerTask() {
            Mixer::applyMix(AxisControllers::throttleOutput, AxisControllers::axisOutputs.pitch, AxisControllers::axisOutputs.roll, AxisControllers::axisOutputs.yaw);
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
        uint8_t pidLoopFrequencyDivider = Config::config.pidLoopFrequencyDivider == 0 ? 1 : Config::config.pidLoopFrequencyDivider;
        auto sequenceSchedule = new Scheduler::SequentialTaskSchedule<3>(FlightControlScheduleName, {gyroTask, pidTask, mixerTask}, {pidLoopFrequencyDivider, 1}, Hardware::gyroscope->sampleRate);

        Scheduler::addTaskRunner(sequenceSchedule);
        Debug::info("Flight Control Tasks configured.");
    }
}
