void resetWatchdogTimer() {
    IWatchdog.reload();
}

void setupMcuHardware(IOConfig ioConfig) {
    // Setup WDT
    IWatchdog.begin(WatchDogTimeout * 1000000);

    auto usbSerial = &SerialUSB;
    usbSerial->begin();
    UARTs.push_back(new StreamSerialPort(usbSerial));

    for (pb_size_t i = 0; i < ioConfig.uartConfigs_count; i++) {
        HardwareSerial* hardwareSerial = nullptr;
        if (ioConfig.uartConfigs[i].has_tx && ioConfig.uartConfigs[i].has_rx) {
            auto txPin = findPin(ioConfig.uartConfigs[i].tx.pinName);
            auto rxPin = findPin(ioConfig.uartConfigs[i].tx.pinName);
            if (txPin != nullptr && rxPin != nullptr) {
                hardwareSerial = new HardwareSerial(rxPin->number, txPin->number);
            }
        }
        // Add to the array even if checks failed to preserve original indexing
        auto serialPort = hardwareSerial == nullptr ? nullptr : new HardwareSerialPort(hardwareSerial);
        UARTs.push_back(serialPort);
    }

    for (pb_size_t i = 0; i < ioConfig.softwareUartConfigs_count; i++) {
        SoftwareSerial* softwareSerial = nullptr;
        if (ioConfig.softwareUartConfigs[i].has_tx && ioConfig.softwareUartConfigs[i].has_rx) {
            auto txPin = findPin(ioConfig.softwareUartConfigs[i].tx.pinName);
            auto rxPin = findPin(ioConfig.softwareUartConfigs[i].tx.pinName);
            if (txPin != nullptr && rxPin != nullptr) {
                softwareSerial = new SoftwareSerial(rxPin->number, txPin->number);
            }
        }
        // Add to the array even if checks failed to preserve original indexing
        auto serialPort = softwareSerial == nullptr ? nullptr : new SoftwareSerialPort(softwareSerial);
        UARTs.push_back(serialPort);
    }

    for (pb_size_t i = 0; i < ioConfig.i2cConfigs_count; i++) {
        TwoWire* i2c = nullptr;
        if (ioConfig.i2cConfigs[i].has_sda && ioConfig.i2cConfigs[i].has_scl) {
            auto sdaPin = findPin(ioConfig.i2cConfigs[i].sda.pinName);
            auto sclPin = findPin(ioConfig.i2cConfigs[i].scl.pinName);
            if (sdaPin != nullptr && sclPin != nullptr) {
                i2c = new TwoWire(sdaPin->number, sclPin->number);
                switch (ioConfig.i2cConfigs[i].speed) {
                    case I2CConfig_Speed__100kHz:
                        i2c->setClock(100000);
                    case I2CConfig_Speed__400kHz:
                        i2c->setClock(400000);
                }
            }
        }
        // Add to the array even if checks failed to preserve original indexing
        I2Cs.push_back(i2c);
    }

    for (pb_size_t i = 0; i < ioConfig.spiConfigs_count; i++) {
        SPIClass* spi = nullptr;
        if (ioConfig.spiConfigs[i].has_mosi && ioConfig.spiConfigs[i].has_miso && ioConfig.spiConfigs[i].has_sck) {
            auto mosiPin = findPin(ioConfig.spiConfigs[i].mosi.pinName);
            auto misoPin = findPin(ioConfig.spiConfigs[i].miso.pinName);
            auto sckPin = findPin(ioConfig.spiConfigs[i].sck.pinName);
            if (mosiPin != nullptr && misoPin != nullptr && sckPin != nullptr) {
                spi = new SPIClass(mosiPin->number, misoPin->number, sckPin->number);
            }
        }
        // Add to the array even if checks failed to preserve original indexing
        SPIs.push_back(spi);
    }
    for (auto spi : SPIs) spi->begin();
    for (auto i2c : I2Cs) i2c->begin();
}
