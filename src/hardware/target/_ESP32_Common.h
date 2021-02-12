Stream* configurationUart = &bluetoothSerial;

HardwareSerial UARTs[] = {
        Serial,
        Serial1,
        Serial2,
};

SPIClass SPIs[] = {
        SPIClass(HSPI),
        SPIClass(VSPI),
};

TwoWire I2Cs[] = {
        Wire,
        Wire1,
};
