void resetWatchdogTimer() {
    IWatchdog.reload();
}

void setupMcuHardware(IOConfig ioConfig) {
    // Setup WDT
    IWatchdog.begin(WatchDogTimeout * 1000000);

    auto usbSerial = &SerialUSB;
    usbSerial->begin();
    UARTs.push_back(UartDescriptor(new StreamSerialPort(usbSerial), "USB VCP"));

    for (pb_size_t i = 0; i < ioConfig.uartConfigs_count; i++) {
        HardwareSerialPort* hardwareSerial = nullptr;
        if (ioConfig.uartConfigs[i].has_tx && ioConfig.uartConfigs[i].has_rx) {
            auto txPin = findPin(ioConfig.uartConfigs[i].tx.pinName);
            auto rxPin = findPin(ioConfig.uartConfigs[i].rx.pinName);
            if (txPin != nullptr && rxPin != nullptr) {
                hardwareSerial = new HardwareSerialPort(new HardwareSerial(rxPin->number, txPin->number));
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
                    default:
                        Log::error(LogTag, "I2C %d speed invalid.", i);
                        break;
                }
            } else {
                Log::error(LogTag, "I2C %d SDA and/or SCL pin does not exist.", i);
            }
        } else {
            Log::error(LogTag, "I2C %d configuration invalid.", i);
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
            } else {
                Log::error(LogTag, "SPI %d MOSI and/or MISO and/or SCK pin does not exist.", i);
            }
        } else {
            Log::error(LogTag, "SPI %d configuration invalid.", i);
        }
        // Add to the array even if checks failed to preserve original indexing
        SPIs.push_back(spi);
    }
    for (auto spi : SPIs) spi->begin();
    for (auto i2c : I2Cs) i2c->begin();
}

// Non volatile memory data layout is:
// * size_t lengthBytes
// * lengthBytes bytes of data
// * 8 bit checksum
constexpr size_t sizeOfSize = sizeof(size_t);
union sizeAsBytes {
    size_t size;
    uint8_t bytes[sizeOfSize];
};

size_t loadData(byte *buffer, size_t maxSize) {
    eeprom_buffer_fill();

    union sizeAsBytes dataSize;
    for (size_t i = 0; i < sizeOfSize; i++) dataSize.bytes[i] = eeprom_buffered_read_byte(i);
    if (dataSize.size > maxSize) {
        Log::error(LogTag, "Error reading non-volatile data: Data size bigger than maximum buffer size.");
        return 0;
    }

    uint8_t crc = 0;
    for (size_t i = 0; i < dataSize.size; i++) {
        buffer[i] = eeprom_buffered_read_byte(i + sizeOfSize);
        crc = CRCUtils::crc8_dvb_s2(crc, buffer[i]);
    }

    uint8_t expectedCrc = eeprom_buffered_read_byte(sizeOfSize + dataSize.size);
    if (crc != expectedCrc) {
        Log::error(LogTag, "Error reading non-volatile data: CRC invalid.");
        return 0;
    }

    return dataSize.size;
}

bool storeData(byte *data, size_t length) {
    if (length + sizeOfSize + 1 > FLASH_PAGE_SIZE) {
        Log::error(LogTag, "Error writing non-volatile data: data is too big.");
        return false;
    }

    union sizeAsBytes dataSize;
    dataSize.size = length;

    for (size_t i = 0; i < sizeOfSize; i++) eeprom_buffered_write_byte(i, dataSize.bytes[i]);

    uint8_t crc = 0;
    for (size_t i = 0; i < length; i++) {
        eeprom_buffered_write_byte(sizeOfSize + i, data[i]);
        crc = CRCUtils::crc8_dvb_s2(crc, data[i]);
    }
    eeprom_buffered_write_byte(sizeOfSize + length, crc);

    eeprom_buffer_flush();
    return true;
}
