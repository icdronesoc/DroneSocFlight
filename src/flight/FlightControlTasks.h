#pragma once

#include <Arduino.h>

namespace FlightControlTasks {
    /**
     * Adds the Flight Control task and Accelerometer read task to the Scheduler.
     * Requires configuration to be loaded.
     *
     * The Flight Control task does the following:
     * * Read Gyro
     * * Run PID loop
     * * Run Mixer and set outputs
     */
    void initialize();
}
