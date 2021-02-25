#pragma once

#include <Arduino.h>
#include "Name.h"
#include "Task.h"

namespace Scheduler {
    constexpr size_t ScheduleAverageLatenessSampleCount = 32;

    class TaskRunner {
    public:
        /**
         * Runs the task if it is time for it to be run.
         */
        virtual void runIfNecessary() = 0;
    };

    /**
     * This defines a way for the scheduler to ask a task if it is ready,
     * and run it as soon as it becomes ready.
     *
     * This is useful for tasks that need to be run at an irregular frequency.
     */
    class AdHocTaskRunner final : public TaskRunner {
    public:
        /**
         * The Task is Ready Function tells the scheduler whether this task is ready to run, for example whether there is Serial data in the buffer waiting to be read.
         * It should be treated like an ISR: it has to complete very quickly to avoid slowing down the whole program, as it is run very frequently.
         */
        using TaskIsReadyFunction = bool();
        AdHocTaskRunner(Task* task, TaskIsReadyFunction taskIsReady);
        void runIfNecessary() override;

    private:
        Task* task;
        TaskIsReadyFunction* taskIsReady;
    };

    /**
     * A task schedule schedules a task to run at a particular frequency.
     */
    class TaskSchedule : public TaskRunner {
    public:
        TaskSchedule(const Name &name, uint32_t frequency);

        const Name& name;

        /**
         * The average of the schedule's lateness (the delay in microseconds between it being supposed to run and it actually running)
         */
        etl::cumulative_moving_average<uint32_t, TaskAverageDurationSampleCount> averageLateness;

        /**
         * The maximum lateness (the delay in microseconds between it being supposed to run and it actually running)
         */
        uint32_t maxLateness;

        void runIfNecessary() final;

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

    /**
     * Schedules a single task to be run, independently of other tasks, at a fixed frequency.
     */
    class IndependentTaskSchedule final : public TaskSchedule {
    public:
        /**
         * Creates a Task Schedule
         * @param task The task to be run
         * @param frequency The frequency at which to run the task
         */
        IndependentTaskSchedule(const Name &name, Task* task, uint32_t frequency);

    protected:
        void run() override;

    private:
        /**
         * The task to be run
         */
        Task* task;
    };

    /**
     * TODO Documentation
     * @tparam TaskCount The number of tasks in the sequence
     */
    template<const size_t TaskCount>
    class SequentialTaskSchedule final : public TaskSchedule {
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
        SequentialTaskSchedule(const Name &name, Task* tasks[TaskCount], const uint8_t frequencyDividers[TaskCount - 1], uint32_t firstTaskFrequency);

    protected:
        void run() override;

    private:
        const Task* tasks[TaskCount];
        const uint8_t frequencyDividers[TaskCount - 1];
        /**
         * Tracks how many times each task has been invoked for the purposes of frequency division.
         * The last task does not need to be tracked as there is no task after it.
         *
         * uint16_t should be enough that even with a high divider overflows eg. 255 aren't a problem.
         * Therefore this doesn't need to be reset to 0.
         */
        uint16_t invocationCount[TaskCount - 1];
    };

    void addTaskRunner(TaskRunner* task);

    void loop();
}