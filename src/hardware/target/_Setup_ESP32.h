constexpr pb_size_t ESP32_MAX_UARTS = 3;
constexpr pb_size_t ESP32_MAX_I2Cs = 2;
constexpr pb_size_t ESP32_MAX_SPIs = 2;

void resetWatchdogTimer() {
    esp_task_wdt_reset();
}

void setupMcuHardware(IOConfig ioConfig) {
    // Setup WDT
    esp_task_wdt_init(WatchDogTimeout, true);
    esp_task_wdt_add(nullptr);

    auto bluetoothSerial = new BluetoothSerial();
    bluetoothSerial->begin("DroneSoc FC");
    UARTs.push_back(new StreamSerialPort(bluetoothSerial));

    for (pb_size_t i = 0; i < min(ESP32_MAX_UARTS, ioConfig.uartConfigs_count); i++) {
        HardwareSerial* hardwareSerial = nullptr;
        if (ioConfig.uartConfigs[i].has_tx && ioConfig.uartConfigs[i].has_rx) {
            auto txPin = pinNameToNumber(ioConfig.uartConfigs[i].tx.pinName);
            auto rxPin = pinNameToNumber(ioConfig.uartConfigs[i].tx.pinName);
            if (txPin != 0 && rxPin != 0) { // TODO better error checking
                hardwareSerial = new HardwareSerial(i);
                hardwareSerial->begin(9600, SERIAL_8N1, rxPin, txPin);
            }
        }
        // Add to the array even if checks failed to preserve original indexing
        auto serialPort = hardwareSerial == nullptr ? nullptr : new HardwareSerialPort(hardwareSerial);
        UARTs.push_back(serialPort);
    }

    for (pb_size_t i = 0; i < ioConfig.softwareUartConfigs_count; i++) {
        SoftwareSerial* softwareSerial = nullptr;
        if (ioConfig.softwareUartConfigs[i].has_tx && ioConfig.softwareUartConfigs[i].has_rx) {
            auto txPin = pinNameToNumber(ioConfig.softwareUartConfigs[i].tx.pinName);
            auto rxPin = pinNameToNumber(ioConfig.softwareUartConfigs[i].tx.pinName);
            if (txPin != 0 && rxPin != 0) { // TODO better error checking
                softwareSerial = new SoftwareSerial(rxPin, txPin);
            }
        }
        // Add to the array even if checks failed to preserve original indexing
        auto serialPort = softwareSerial == nullptr ? nullptr : new SoftwareSerialPort(softwareSerial);
        UARTs.push_back(serialPort);
    }

    for (pb_size_t i = 0; i < min(ESP32_MAX_I2Cs, ioConfig.i2cConfigs_count); i++) {
        TwoWire* i2c = nullptr;
        if (ioConfig.i2cConfigs[i].has_sda && ioConfig.i2cConfigs[i].has_scl) {
            auto sdaPin = pinNameToNumber(ioConfig.i2cConfigs[i].sda.pinName);
            auto sclPin = pinNameToNumber(ioConfig.i2cConfigs[i].scl.pinName);
            if (sdaPin != 0 && sclPin != 0) { // TODO better error checking
                i2c = new TwoWire(i);
                int frequency = 0;
                switch (ioConfig.i2cConfigs[i].speed) {
                    case I2CConfig_Speed__100kHz:
                        frequency = 100000;
                    case I2CConfig_Speed__400kHz:
                        frequency = 400000;
                }
                i2c->begin(sdaPin, sclPin, frequency);
            }
        }
        // Add to the array even if checks failed to preserve original indexing
        I2Cs.push_back(i2c);
    }

    for (pb_size_t i = 0; i < min(ESP32_MAX_SPIs, ioConfig.spiConfigs_count); i++) {
        SPIClass* spi = nullptr;
        if (ioConfig.spiConfigs[i].has_mosi && ioConfig.spiConfigs[i].has_miso && ioConfig.spiConfigs[i].has_sck) {
            auto mosiPin = pinNameToNumber(ioConfig.spiConfigs[i].mosi.pinName);
            auto misoPin = pinNameToNumber(ioConfig.spiConfigs[i].miso.pinName);
            auto sckPin = pinNameToNumber(ioConfig.spiConfigs[i].sck.pinName);
            if (mosiPin != 0 && misoPin != 0 && sckPin != 0) { // TODO better error checking
                spi = new SPIClass(i == 0 ? HSPI : VSPI);
                spi->begin(sckPin, misoPin, mosiPin);
            }
        }
        // Add to the array even if checks failed to preserve original indexing
        SPIs.push_back(spi);
    }
}
