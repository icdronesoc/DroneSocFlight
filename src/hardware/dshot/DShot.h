#pragma once

#include <Arduino.h>

namespace DShot {
    class DShotOutput {
    public:
        virtual void sendPacket(uint16_t packet) = 0;
    };

    enum class DShotSpeed {
        DShot1200,
        DShot600,
        DShot300,
        DShot150,
    };

    /**
     * Creates a DShot Output for a specified pin.
     * This will setup the pin for DShot output.
     * @param pin The pin to output DShot on
     * @param speed The DShot speed for the output
     * @return The DShot Output, or nullptr if allocating or setting up the output failed.
     */
    DShotOutput* createDShotOutput(uint32_t pin, DShotSpeed speed);
}
