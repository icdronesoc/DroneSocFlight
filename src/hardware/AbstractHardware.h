#pragma once

#include <Arduino.h>

namespace Hardware {
    struct ThreeAxisData { // TODO define what is x, y, z for gyro/accelerometer
        double x;
        double y;
        double z;
    };

    /**
     * Initialized in constructor.
     */
    class Accelerometer {
    public:
        explicit Accelerometer(uint32_t sampleRate) : sampleRate(sampleRate) {}

        /**
         * @return Data for each axis in Gs
         */
        virtual ThreeAxisData getAccelerationData() = 0;

        /**
         * The sample rate of the sensor, in Hz
         */
        uint32_t sampleRate;
    };

    /**
     * Initialized in constructor.
     */
    class Gyroscope {
    public:
        explicit Gyroscope(uint32_t sampleRate) : sampleRate(sampleRate) {}

        /**
         * @return Data for each axis in degrees per second
         */
        virtual ThreeAxisData getRotationData() = 0;

        /**
         * The sample rate of the sensor, in Hz
         */
        uint32_t sampleRate;
    };

    /**
     * Initialized in constructor.
     */
    class Motor {
    public:
        /**
         * @param output The speed the motor should be set to, ranging from -1 (full reverse, if supported) to 1 (full forwards)
         */
        virtual void setOutput(double output) = 0;
    };

    class Servo {
    public:
        /**
         * @param output The position the Servo should move to, ranging from -1 to 1
         */
        virtual void setOutput(double output) = 0;
    };
}
