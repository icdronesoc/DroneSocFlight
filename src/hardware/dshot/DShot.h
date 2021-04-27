#pragma once

#include <Arduino.h>
#include "etl/vector.h"

namespace DShot {
    enum class Speed {
        DShot1200,
        DShot600,
        DShot300,
        DShot150,
    };

    enum class Command {
        // Commands below are only executed when motors are stopped
        MOTOR_STOP = 0,
        BEEP1 = 1,
        BEEP2 = 2,
        BEEP3 = 3,
        BEEP4 = 4,
        BEEP5 = 5,
        ESC_INFO = 6,
        SPIN_DIRECTION_1 = 7,
        SPIN_DIRECTION_2 = 8,
        SWITCH_3D_MODE_OFF = 9,
        SWITCH_3D_MODE_ON = 10,
        SETTINGS_REQUEST = 11,
        SAVE_SETTINGS = 12,
        SPIN_DIRECTION_NORMAL = 20,
        SPIN_DIRECTION_REVERSED = 21,
        LED0_ON = 22,
        LED1_ON = 23,
        LED2_ON = 24,
        LED3_ON = 25,
        LED0_OFF = 26,
        LED1_OFF = 27,
        LED2_OFF = 28,
        LED3_OFF = 29,
        TOGGLE_AUDIO_STREAM_MODE = 30,
        TOGGLE_SILENT_MODE = 31,
        SIGNAL_LINE_TELEMETRY_DISABLE = 32,
        SIGNAL_LINE_TELEMETRY_ENABLE = 33,
        SIGNAL_LINE_CONTINUOUS_ERPM_TELEMETRY = 34,
        SIGNAL_LINE_CONTINUOUS_ERPM_PERIOD_TELEMETRY = 35,

        // Commands below are executed at any time
        SIGNAL_LINE_TEMPERATURE_TELEMETRY = 42,
        SIGNAL_LINE_VOLTAGE_TELEMETRY = 43,
        SIGNAL_LINE_CURRENT_TELEMETRY = 44,
        SIGNAL_LINE_CONSUMPTION_TELEMETRY = 45,
        SIGNAL_LINE_ERPM_TELEMETRY = 46,
        SIGNAL_LINE_ERPM_PERIOD_TELEMETRY = 47,
    };

    class Driver {
    public:
        virtual void sendPacket(uint16_t packet) = 0;
    };

    class Output {
    public:
        Output(Driver& driver, bool isBidirectional);
        void writeThrottleValue(double throttle);
        void sendCommand(Command command);

    private:
        Driver& driver;
        bool isBidirectional;
    };

    constexpr size_t MaxDShotOutputs = 8;
    extern etl::vector<Output*, MaxDShotOutputs> allOutputs;

    /**
     * Creates a DShot Driver for a specified pin. Must not be called by client code, use createOutput().
     * This must be implemented per-platform.
     * This will setup the pin for DShot output.
     * @param pin The pin to output DShot on
     * @param speed The DShot speed for the output
     * @param bidirectionalTelemetry Whether to enable bidirectional telemetry
     * @return The DShot Output Driver, or nullptr if setting up the driver failed.
     */
    Driver* createDriver(uint32_t pin, Speed speed, bool bidirectionalTelemetry);

    /**
     * Creates a DShot Output for a specified pin.
     * This will setup the pin for DShot output.
     * @param pin The pin to output DShot on
     * @param speed The DShot speed for the output
     * @param bidirectional Whether the ESC is in bidirectional mode
     * @return The DShot Output, or nullptr if allocating or setting up the output failed.
     */
    Output* createOutput(uint32_t pin, Speed speed, bool isBidirectional, bool bidirectionalTelemetry);

    /**
     * Sends a DShot command to all motors.
     * @param command
     */
    void sendCommandToAllOutputs(Command command);
}
