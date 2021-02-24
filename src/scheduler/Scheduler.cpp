#include "Scheduler.h"
#include <etl/vector.h>

namespace Scheduler {
    namespace { // private
        constexpr size_t MaxNumberOfTaskRunners = 32;

        etl::vector<TaskRunner*, MaxNumberOfTaskRunners> taskRunners;
    }

    AdHocTask::AdHocTask(Task task, AdHocTask::TaskIsReadyFunction taskIsReady) : task(task), taskIsReady(taskIsReady) {}

    void AdHocTask::runIfNecessary() {
        if (this->taskIsReady()) this->task.run();
    }

    TaskSchedule::TaskSchedule(const ScheduleName &name, uint32_t frequency) : name(name), averageLateness(0), maxLateness(0), period(100000 / frequency), lastRunTime(0) {}

    void TaskSchedule::runIfNecessary() {
        uint32_t now = micros();
        uint32_t elapsedTime = now - this->lastRunTime;
        if (elapsedTime >= this->period) {
            auto lateness = elapsedTime - this->period;
            if (this->maxLateness < lateness) this->maxLateness = lateness;
            this->averageLateness.add(lateness);
            this->lastRunTime = now;
            this->run();
        }
    }

    IndependentTaskSchedule::IndependentTaskSchedule(const ScheduleName &name, const Task task, const uint32_t frequency) : TaskSchedule(name, frequency), task(task) {}

    void IndependentTaskSchedule::run() {
        this->task.run();
    }

    template<const size_t taskCount>
    SequentialTaskSchedule<taskCount>::SequentialTaskSchedule(const ScheduleName &name, const Task tasks[taskCount], const uint8_t frequencyDividers[taskCount-1], uint32_t firstTaskFrequency) : TaskSchedule(name, firstTaskFrequency), tasks(tasks), frequencyDividers(frequencyDividers) {}

    template<const size_t taskCount>
    void SequentialTaskSchedule<taskCount>::run() {
        // Loop through all tasks
        for (size_t i = 0; i < taskCount; i++) {
            // Run the task
            this->tasks[i].run();

            // Exit if this is the last task. The last task does not have an invocation count so this has to be done first.
            if (i+1 == taskCount) return;

            // Increment the invocation count
            this->invocationCount[i]++;

            // If the invocation count % the divider is not 0, it's not time to run the next task yet.
            if (this->invocationCount % this->frequencyDividers[i] != 0) return;
        }
    }

    void addTaskRunner(TaskRunner* taskRunner) {
        taskRunners.push_back(taskRunner);
    }

    void loop() {
        for (auto & taskRunner : taskRunners) {
            taskRunner->runIfNecessary();
        }
    }
}
