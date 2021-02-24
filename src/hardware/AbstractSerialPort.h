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

        virtual Stream* operator->() = 0;
        Stream& operator*() { return *operator->(); }
    };

    class HardwareSerialPort final : public SerialPort {
    private:
        HardwareSerial *hardwareSerial;
#ifdef PLATFORM_ESP32
        uint32_t txPin, rxPin;
#endif

    public:
        /**
         * Wraps a HardwareSerial, WHICH MUST NOT BE NULL.
         */
#ifdef PLATFORM_ESP32
        HardwareSerialPort(HardwareSerial *hardwareSerial, uint32_t txPin, uint32_t rxPin) : hardwareSerial(hardwareSerial), txPin(txPin), rxPin(rxPin) {}
#else
        HardwareSerialPort(HardwareSerial *hardwareSerial) : hardwareSerial(hardwareSerial) {}
#endif

        void begin(unsigned long baud) override {
#ifdef PLATFORM_ESP32
            hardwareSerial->begin(baud, SERIAL_8N1, rxPin, txPin);
#else
            hardwareSerial->begin(baud);
#endif
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
