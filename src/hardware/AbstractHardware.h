#pragma once

#include <Arduino.h>

typedef struct {
    double x;
    double y;
    double z;
} ThreeAxisData;

class HasInitializer {
public:
    virtual void initialize() = 0;
};

class Accelerometer : public HasInitializer {
public:
    /**
     * @return Data for each axis in TODO units
     */
    virtual ThreeAxisData getAccelerationData() = 0;
};

class Gyroscope : public HasInitializer {
public:
    /**
     * @return Data for each axis in degrees per second
     */
    virtual ThreeAxisData getRotationData() = 0;
};

class Motor : public HasInitializer {
public:
    /**
     * @param output The speed the motor should be set to, ranging from 0 to 2047
     */
    virtual void setOutput(int16_t output) = 0;
};

class Servo : public HasInitializer {
public:
    /**
     * @param output The position the Servo should move to, ranging from -1024 to 1023
     */
    virtual void setOutput(int16_t output) = 0;
};
