#pragma once

#include <Arduino.h>

typedef struct {
    double x;
    double y;
    double z;
} ThreeAxisData;

class HasInitializer {
    virtual void initialize() = 0;
};

class Accelerometer : HasInitializer {
    /**
     * @return Data for each axis in TODO
     */
    virtual ThreeAxisData getAccelerationData() = 0;
};

class Gyroscope : HasInitializer {
    /**
     * @return Data for each axis in degrees per second
     */
    virtual ThreeAxisData getRotationData() = 0;
};

class MotorOutput : HasInitializer {
    virtual void setOutput(int16_t output) = 0;
};
