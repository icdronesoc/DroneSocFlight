#pragma once

#include <Arduino.h>
#include "Name.h"
#include <etl/cumulative_moving_average.h>

namespace Scheduler {
    constexpr size_t TaskAverageDurationSampleCount = 32;

    using TaskFunction = void();

    /**
     * A task is an arbitrary function and a name for the function.
     * This class also contains capabilities for tracking how long a task takes to run.
     */
    class Task {
    public:
        /**
         * Creates a task.
         * @param task The function that is run when the task is run
         * @param name The name of the task
         */
        Task(TaskFunction* task, const Name& name);
        const Name& name;
        etl::cumulative_moving_average<uint32_t, TaskAverageDurationSampleCount> averageRunTime;
        uint32_t maxRunTime;

        /**
         * Runs the task and records its performance data.
         */
        void run();

    private:
        TaskFunction* task;
    };
}

