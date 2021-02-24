#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>

namespace IO {
/**
 * SerialPort solves the issue that there is no common superclass for Hardware, Software, and other (USB / Bluetooth) hardwareSerial ports.
 */
    class SerialPort {
    public:
        /**
         * Begin communication
         * @param baud The baud rate to use. Some implementations don't use this.
         */
        virtual void begin(unsigned long baud) = 0;

        virtual Stream *operator->() = 0;
    };

    class HardwareSerialPort final : public SerialPort {
    private:
        HardwareSerial *hardwareSerial;

    public:
        /**
         * Wraps a HardwareSerial, WHICH MUST NOT BE NULL.
         */
        explicit HardwareSerialPort(HardwareSerial *hardwareSerial) : hardwareSerial(hardwareSerial) {}

        void begin(unsigned long baud) override {
            hardwareSerial->begin(baud); // TODO This breaks ESP32 as pin numbers are set in begin()
        }

        Stream *operator->() override {
            return hardwareSerial;
        }
    };

    class SoftwareSerialPort final : public SerialPort {
    private:
        SoftwareSerial *softwareSerial;

    public:
        /**
         * Wraps a SoftwareSerial, WHICH MUST NOT BE NULL.
         */
        explicit SoftwareSerialPort(SoftwareSerial *softwareSerial) : softwareSerial(softwareSerial) {}

        void begin(unsigned long baud) override {
            softwareSerial->begin(static_cast<long>(baud));
        }

        Stream *operator->() override {
            return softwareSerial;
        }
    };

    class StreamSerialPort final : public SerialPort {
    private:
        Stream *stream;

    public:
        /**
         * Wraps a Stream, WHICH MUST NOT BE NULL.
         */
        explicit StreamSerialPort(Stream *stream) : stream(stream) {}

        void begin(unsigned long baud) override {
            // Do nothing
        }

        Stream *operator->() override {
            return stream;
        }
    };
}
