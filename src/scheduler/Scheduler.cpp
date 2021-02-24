#include "Scheduler.h"
#include <etl/vector.h>

namespace Scheduler {
    namespace { // private
        constexpr size_t MaxNumberOfTaskSchedules = 32;

        etl::vector<TaskSchedule*, MaxNumberOfTaskSchedules> taskSchedules;
    }

    TaskSchedule::TaskSchedule(uint32_t frequency) : period(100000 / frequency), lastRunTime(0) {}

    void TaskSchedule::runIfNecessary() {
        uint32_t now = micros();
        if (now - this->lastRunTime >= this->period) {
            this->lastRunTime = now;
            this->run();
        }
    }

    IndependentTaskSchedule::IndependentTaskSchedule(const Task task, const uint32_t frequency) : TaskSchedule(frequency), task(task) {}

    void IndependentTaskSchedule::run() {
        this->task.run();
    }

    template<const size_t taskCount>
    SequentialTaskSchedule<taskCount>::SequentialTaskSchedule(const Task tasks[taskCount], const uint8_t frequencyDividers[taskCount-1], uint32_t firstTaskFrequency) : TaskSchedule(firstTaskFrequency), tasks(tasks), frequencyDividers(frequencyDividers) {}

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

    void addTask(TaskSchedule* task) {
        taskSchedules.push_back(task);
    }

    void loop() {
        for (auto & task : taskSchedules) {
            task->runIfNecessary();
        }
    }
}
