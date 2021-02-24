#pragma once

#include <Arduino.h>
#include <etl/string.h>
#include <etl/cumulative_moving_average.h>

namespace Scheduler {
    constexpr size_t MaxNumberOfTasks = 256;
    constexpr size_t TaskNameMaxSize = 8;
    constexpr size_t TaskAverageDurationSampleCount = 32;

    using TaskFunction = void();
    using TaskName = etl::string<TaskNameMaxSize>;

    class Task {
    public:
        Task(TaskFunction* task, const TaskName& name, uint32_t frequency);
        const TaskName& name;
        void runIfNecessary();

    private:
        TaskFunction* task;
        /**
         * The period between task runs, in microseconds.
         */
        const uint32_t period;
        uint32_t lastRunTime;
        /**
         * The last time this task was run
         */
        etl::cumulative_moving_average<uint32_t, TaskAverageDurationSampleCount> averageRunTime;
        uint32_t maxRunTime;
    };

    void addTask(Task task);

    void loop();
}
