#pragma once

#include <Arduino.h>

namespace Hardware {
    struct ThreeAxisData {
        double x;
        double y;
        double z;
    };

    /**
     * Initialized in constructor.
     */
    class Accelerometer {
    public:
        Accelerometer(uint32_t sampleRate) : sampleRate(sampleRate) {}

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
        Gyroscope(uint32_t sampleRate) : sampleRate(sampleRate) {}

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
         * @param output The speed the motor should be set to, ranging from 0 to 2047
         */
        virtual void setOutput(int16_t output) = 0;
    };

    class Servo {
    public:
        /**
         * @param output The position the Servo should move to, ranging from -1024 to 1023
         */
        virtual void setOutput(int16_t output) = 0;
    };
}
