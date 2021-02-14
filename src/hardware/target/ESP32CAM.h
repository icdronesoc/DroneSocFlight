#include "_ESP32_Common.h"

Pin pins[] = {
        { "0", 0 },
        { "1", 1 },
        { "2", 2 },
        { "3", 3 },
        { "4", 4 },
        { "12", 12 },
        { "13", 13 },
        { "14", 14 },
        { "15", 15 },
        { "16", 16 },
};

void setupMcuHardware(IOConfig ioConfig) {
    setupEspHardware();
    // TODO use ioConfig

    // https://randomnerdtutorials.com/esp32-cam-ai-thinker-pinout/
    // https://github.com/SeeedDocument/forum_doc/raw/master/reg/ESP32_CAM_V1.6.pdf
}
