#include "Scheduler.h"
#include <etl/vector.h>

namespace Scheduler {
    namespace { // private
        etl::vector<Task, MaxNumberOfTasks> tasks;
    }

    Task::Task(TaskFunction* task, const TaskName& name, uint32_t frequency) : name(name), task(task), period(1000000 / frequency), lastRunTime(0), averageRunTime(period), maxRunTime(0) {}

    void Task::runIfNecessary() {
        uint32_t start = micros();
        if (start - this->lastRunTime > this->period) {
            this->lastRunTime = start;
            this->task();
            uint32_t runTime = micros() - start;
            if (this->maxRunTime < runTime) this->maxRunTime = runTime;
            this->averageRunTime.add(runTime);
        }
    }

    void addTask(Task task) {
        tasks.push_back(task);
    }

    void loop() {
        for (auto & task : tasks) {
            task.runIfNecessary();
        }
    }
}
