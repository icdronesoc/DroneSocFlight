const Stream* configurationUart = &bluetoothSerial;

const HardwareSerial UARTs[] = {
        Serial,
        Serial1,
        Serial2,
};

const SPIClass SPIs[] = {
        SPIClass(HSPI),
        SPIClass(VSPI),
};
