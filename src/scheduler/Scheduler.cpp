#include "Scheduler.h"

namespace Scheduler {
    namespace { // private
        etl::vector<TaskRunner*, MaxNumberOfTaskRunners> taskRunners;
    }

    etl::vector<Task*, MaxNumberOfTasks> allTasks;
    etl::vector<TaskSchedule*, MaxNumberOfTaskRunners> allSchedules;

    AdHocTaskRunner::AdHocTaskRunner(Task* task, AdHocTaskRunner::TaskIsReadyFunction taskIsReady) : task(task), taskIsReady(taskIsReady) {}

    void AdHocTaskRunner::runIfNecessary() {
        if (this->taskIsReady()) this->task->run();
    }

    TaskSchedule::TaskSchedule(const Name &name, uint32_t frequency) : name(name), averageLateness(0), maxLateness(0), period(100000 / frequency), lastRunTime(0) {
        allSchedules.push_back(this);
    }

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

    IndependentTaskSchedule::IndependentTaskSchedule(const Name &name, Task* task, const uint32_t frequency) : TaskSchedule(name, frequency), task(task) {}

    void IndependentTaskSchedule::run() {
        this->task->run();
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
