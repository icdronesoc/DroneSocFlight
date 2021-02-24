#pragma once

#include <Arduino.h>
#include "Task.h"

namespace Scheduler {
    class TaskSchedule {
    public:
        // TODO track things like average and max delay in schedule being run
        explicit TaskSchedule(uint32_t frequency);

        /**
         * Runs the task if it is time for it to be run.
         */
        void runIfNecessary();

    protected:
        /**
         * Runs the task. Must be implemented by subclasses.
         */
        virtual void run() = 0;

    private:
        /**
         * The period between task runs, in microseconds.
         */
        const uint32_t period;

        /**
         * The last time this task was run
         */
        uint32_t lastRunTime;
    };

    // TODO create AdHocTask

    /**
     * Schedules a single task to be run, independently of other tasks, at a fixed frequency.
     */
    class IndependentTaskSchedule : public TaskSchedule {
    public:
        /**
         * Creates a Task Schedule
         * @param task The task to be run
         * @param frequency The frequency at which to run the task
         */
        IndependentTaskSchedule(Task task, uint32_t frequency);

    protected:
        void run() override;

    private:
        /**
         * The task to be run
         */
        Task task;
    };

    template<const size_t taskCount>
    class SequentialTaskSchedule : public TaskSchedule {
    public:
        /**
         * Creates a sequential task schedule.
         * @param tasks The tasks to be run in sequence
         * @param frequencyDividers The frequency dividers for each task.
         * The first task will never have a frequency divider, so the first
         * index is the frequency divider for the second task, the second
         * index is the frequency divider for the third task, etc...
         * @param firstTaskFrequency The frequency to run the first task at
         */
        SequentialTaskSchedule(const Task tasks[taskCount], const uint8_t frequencyDividers[taskCount-1], uint32_t firstTaskFrequency);

    protected:
        void run() override;

    private:
        Task tasks[taskCount];
        const uint8_t frequencyDividers[taskCount-1];
        /**
         * Tracks how many times each task has been invoked for the purposes of frequency division.
         * The last task does not need to be tracked as there is no task after it.
         *
         * uint16_t should be enough that even with a high divider overflows eg. 255 aren't a problem.
         * Therefore this doesn't need to be reset to 0.
         */
        uint16_t invocationCount[taskCount-1];
    };

    void addTask(TaskSchedule* task);

    void loop();
}
