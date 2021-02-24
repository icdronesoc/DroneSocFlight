#pragma once

#include <Arduino.h>
#include <etl/vector.h>

namespace RC {
    constexpr size_t MaxChannelCount = 16;

    using Channels = etl::vector<uint16_t, MaxChannelCount>;

    class Driver {
    public:
        explicit Driver(size_t channelCount) : channelCount(channelCount) {}
        size_t channelCount;

        /**
         * Gets an RC Frame.
         * This is called in a loop. It should try to get a packet, and return any telemetry data if needed.
         *
         * @param channels The RC channel data that should be updated if a frame is received
         * @return Whether a frame was received
         */
        virtual bool getFrame(Channels& channels) = 0;
    };

    /**
     * Channels range from 0-2047.
     */
    extern Channels channels;

    /**
     * Initializes the RC driver using the current configuration
     */
    void initialize();

    void loopTask();

    /**
     * @return The number of milliseconds since the last received frame
     */
    uint32_t timeSinceLastFrame();
}
