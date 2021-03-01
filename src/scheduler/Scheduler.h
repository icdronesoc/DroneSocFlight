#pragma once

#include <Arduino.h>
#include "Name.h"
#include "Task.h"
#include "etl/vector.h"
#include "etl/array.h"

namespace Scheduler {
    constexpr size_t MaxNumberOfTasks = 32;
    constexpr size_t MaxNumberOfTaskRunners = 32;
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
        etl::cumulative_moving_average<uint32_t, ScheduleAverageLatenessSampleCount> averageLateness;

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
     * A Sequential Task Schedule allows tasks that are very tightly coupled
     * to be run in the correct order, whilst being profiled individually.
     *
     * A task list is established, which contains the tasks to be run.
     * Tasks are run in order, one after the order.
     *
     * If a frequency divider is set for a task, this task's frequency is the
     * previous task's frequency divided by the divider. So, if you had 3 tasks,
     * A B and C, each had a frequency divider of 2, and the first task frequency
     * was set to 8kHz, A would run at 8kHz, B would run at 4kHz (every other run
     * of A), and C would run at 2kHz (every other run of B).
     *
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
        SequentialTaskSchedule(const Name &name, const etl::array<Task*, TaskCount> tasks, const etl::array<uint8_t, TaskCount-1> frequencyDividers, const uint32_t firstTaskFrequency) : TaskSchedule(name, firstTaskFrequency), tasks(tasks), frequencyDividers(frequencyDividers), invocationCount(etl::array<uint16_t, TaskCount-1>()) {}

    protected:
        void run() override {
            // Loop through all tasks
            for (size_t i = 0; i < TaskCount; i++) {
                // Run the task
                this->tasks[i]->run();

                // Exit if this is the last task. The last task does not have an invocation count so this has to be done first.
                if (i+1 == TaskCount) return;

                // Increment the invocation count
                this->invocationCount[i]++;

                // If the invocation count % the divider is not 0, it's not time to run the next task yet.
                if (this->invocationCount[i] % this->frequencyDividers[i] != 0) return;
            }
        }

    private:
        const etl::array<Task*, TaskCount> tasks;
        const etl::array<uint8_t, TaskCount-1> frequencyDividers;
        /**
         * Tracks how many times each task has been invoked for the purposes of frequency division.
         * The last task does not need to be tracked as there is no task after it.
         *
         * uint16_t should be enough that even with a high divider overflows eg. 255 aren't a problem.
         * Therefore this doesn't need to be reset to 0.
         */
        etl::array<uint16_t, TaskCount-1> invocationCount;
    };

    /**
     * A list of all individual tasks, accessible for profiling.
     */
    extern etl::vector<Task*, MaxNumberOfTasks> allTasks;

    /**
     * A list of all individual schedules, accessible for profiling.
     */
    extern etl::vector<TaskSchedule*, MaxNumberOfTaskRunners> allSchedules;

    void addTaskRunner(TaskRunner* task);

    void loop();
}
