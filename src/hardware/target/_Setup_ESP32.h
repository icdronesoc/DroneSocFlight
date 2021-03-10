constexpr pb_size_t ESP32_MAX_UARTS = 3;
constexpr pb_size_t ESP32_MAX_I2Cs = 2;
constexpr pb_size_t ESP32_MAX_SPIs = 2;

Preferences preferences;

void resetWatchdogTimer() {
    esp_task_wdt_reset();
}

void setupMcuHardware(IOConfig ioConfig) {
    // Setup preferences
    preferences.begin("config", false);

    // Setup WDT
    esp_task_wdt_init(WatchDogTimeout, true);
    esp_task_wdt_add(nullptr);

    auto bluetoothSerial = new BluetoothSerial();
    bluetoothSerial->begin("DroneSoc FC");
    UARTs.push_back(UartDescriptor(new StreamSerialPort(bluetoothSerial), "Bluetooth"));

    for (pb_size_t i = 0; i < min(ESP32_MAX_UARTS, ioConfig.uartConfigs_count); i++) {
        HardwareSerialPort* hardwareSerial = nullptr;
        if (ioConfig.uartConfigs[i].has_tx && ioConfig.uartConfigs[i].has_rx) {
            auto txPin = findPin(ioConfig.uartConfigs[i].tx.pinName);
            auto rxPin = findPin(ioConfig.uartConfigs[i].rx.pinName);
            if (txPin != nullptr && rxPin != nullptr) {
                hardwareSerial = new HardwareSerialPort(new HardwareSerial(i), txPin->number, rxPin->number);
            } else {
                Log::error(LogTag, "Hardware Serial %d TX and/or RX pin does not exist.", i);
            }
        } else {
            Log::error(LogTag, "Hardware Serial %d configuration invalid.", i);
        }
        // Add to the array even if checks failed to preserve original indexing
        UARTs.push_back(UartDescriptor(hardwareSerial, ioConfig.uartConfigs[i].name));
    }

    for (pb_size_t i = 0; i < ioConfig.softwareUartConfigs_count; i++) {
        SoftwareSerialPort* softwareSerial = nullptr;
        if (ioConfig.softwareUartConfigs[i].has_tx && ioConfig.softwareUartConfigs[i].has_rx) {
            auto txPin = findPin(ioConfig.softwareUartConfigs[i].tx.pinName);
            auto rxPin = findPin(ioConfig.softwareUartConfigs[i].rx.pinName);
            if (txPin != nullptr && rxPin != nullptr) {
                softwareSerial = new SoftwareSerialPort(new SoftwareSerial(rxPin->number, txPin->number));
            } else {
                Log::error(LogTag, "Software Serial %d TX and/or RX pin does not exist.", i);
            }
        } else {
            Log::error(LogTag, "Software Serial %d configuration invalid.", i);
        }
        // Add to the array even if checks failed to preserve original indexing
        UARTs.push_back(UartDescriptor(softwareSerial, ioConfig.softwareUartConfigs[i].name));
    }

    for (pb_size_t i = 0; i < min(ESP32_MAX_I2Cs, ioConfig.i2cConfigs_count); i++) {
        TwoWire* i2c = nullptr;
        if (ioConfig.i2cConfigs[i].has_sda && ioConfig.i2cConfigs[i].has_scl) {
            auto sdaPin = findPin(ioConfig.i2cConfigs[i].sda.pinName);
            auto sclPin = findPin(ioConfig.i2cConfigs[i].scl.pinName);
            if (sdaPin != nullptr && sclPin != nullptr) {
                i2c = new TwoWire(i);
                int frequency = 0;
                switch (ioConfig.i2cConfigs[i].speed) {
                    case I2CConfig_Speed__100kHz:
                        frequency = 100000;
                        break;
                    case I2CConfig_Speed__400kHz:
                        frequency = 400000;
                        break;
                    default:
                        Log::error(LogTag, "I2C %d speed invalid.", i);
                        break;
                }
                i2c->begin(sdaPin->number, sclPin->number, frequency);
            } else {
                Log::error(LogTag, "I2C %d SDA and/or SCL pin does not exist.", i);
            }
        } else {
            Log::error(LogTag, "I2C %d configuration invalid.", i);
        }
        // Add to the array even if checks failed to preserve original indexing
        I2Cs.push_back(i2c);
    }

    for (pb_size_t i = 0; i < min(ESP32_MAX_SPIs, ioConfig.spiConfigs_count); i++) {
        SPIClass* spi = nullptr;
        if (ioConfig.spiConfigs[i].has_mosi && ioConfig.spiConfigs[i].has_miso && ioConfig.spiConfigs[i].has_sck) {
            auto mosiPin = findPin(ioConfig.spiConfigs[i].mosi.pinName);
            auto misoPin = findPin(ioConfig.spiConfigs[i].miso.pinName);
            auto sckPin = findPin(ioConfig.spiConfigs[i].sck.pinName);
            if (mosiPin != nullptr && misoPin != nullptr && sckPin != nullptr) {
                spi = new SPIClass(i == 0 ? HSPI : VSPI);
                spi->begin(sckPin->number, misoPin->number, mosiPin->number);
            } else {
                Log::error(LogTag, "SPI %d MOSI and/or MISO and/or SCK pin does not exist.", i);
            }
        } else {
            Log::error(LogTag, "SPI %d configuration invalid.", i);
        }
        // Add to the array even if checks failed to preserve original indexing
        SPIs.push_back(spi);
    }
}

size_t loadData(byte *buffer, size_t maxSize) {
    return preferences.getBytes("config", buffer, maxSize);
}

bool storeData(byte *data, size_t length) {
    return preferences.putBytes("config", data, length) > 0;
}
