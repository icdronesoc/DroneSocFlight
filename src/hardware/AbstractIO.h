#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>

/**
 * SerialPort solves the issue that there is no common superclass for Hardware, Software, and other (USB / Bluetooth) serial ports.
 */
class SerialPort {
    /**
     * Begin communication
     * @param baud The baud rate to use. Some implementations don't use this.
     */
    virtual void begin(unsigned long baud) = 0;
    virtual Stream* operator->() = 0;
};

class HardwareSerialPort : public SerialPort {
private:
    HardwareSerial* serial;

public:
    /**
     * Wraps a HardwareSerial, WHICH MUST NOT BE NULL.
     */
    explicit HardwareSerialPort(HardwareSerial* s) : serial(s) {}

    void begin(unsigned long baud) override {
        serial->begin(baud);
    }

    Stream* operator->() override {
        return serial;
    }
};

class SoftwareSerialPort : public SerialPort {
private:
    SoftwareSerial* serial;

public:
    /**
     * Wraps a SoftwareSerial, WHICH MUST NOT BE NULL.
     */
    explicit SoftwareSerialPort(SoftwareSerial* s) : serial(s) {}

    void begin(unsigned long baud) override {
        serial->begin(static_cast<long>(baud));
    }

    Stream* operator->() override {
        return serial;
    }
};

class StreamSerialPort : public SerialPort {
private:
    Stream* serial;

public:
    /**
     * Wraps a Stream, WHICH MUST NOT BE NULL.
     */
    explicit StreamSerialPort(Stream* s) : serial(s) {}

    void begin(unsigned long baud) override {
        // Do nothing
    }

    Stream* operator->() override {
        return serial;
    }
};
