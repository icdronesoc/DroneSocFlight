#include "FlightControlTasks.h"
#include "hardware/Hardware.h"
#include "scheduler/Scheduler.h"

namespace FlightControlTasks {
    namespace { // private
        const Scheduler::Name FlightControlScheduleName = "Flight Control";

        const Scheduler::Name GyroscopeTaskName = "Gyroscope";

        void readGyroscope() {
            // TODO
        }

        const Scheduler::Name AccelerometerTaskName = "Accelerometer";

        void readAccelerometer() {
            // TODO
        }

        const Scheduler::Name PidTaskName = "PID Loop";

        void doPidTask() {
            // TODO
        }

        const Scheduler::Name MixerTaskName = "Mixer";

        void doMixerTask() {
            // TODO
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
