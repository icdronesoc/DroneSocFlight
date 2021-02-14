#include "Hardware.h"
#include "McuHardware.h"

namespace Hardware {
    Accelerometer* accelerometer = nullptr;
    Gyroscope* gyroscope = nullptr;
    etl::vector<MotorOutput*, maxMotorCount> motors;
    etl::vector<ServoOutput*, maxServoCount> servos;

    void initialize(Config::HardwareConfiguration hardwareConfiguration, Config::SoftwareConfiguration softwareConfiguration) {
        if (hardwareConfiguration.has_busConfig) {
            for (int i = 0; i < McuHardware::i2cCount; i++) {
                switch (hardwareConfiguration.busConfig.i2cSpeed) {
                    case BusConfig_I2CSpeed__100KHz:
                        McuHardware::I2Cs[i].setClock(100000);
                        break;
                }
            }
        }

        if (hardwareConfiguration.has_accelerometerConfig) {
            switch(hardwareConfiguration.accelerometerConfig.which_driverConfig) {
                case AccelerometerConfig_mpuSpi_tag:
                    break;
                case AccelerometerConfig_mpuI2c_tag:
                    break;
            }
            if (accelerometer != nullptr) accelerometer->initialize();
        }

        if (hardwareConfiguration.has_gyroscopeConfig) {
            switch(hardwareConfiguration.gyroscopeConfig.which_driverConfig) {
                case GyroscopeConfig_mpuSpi_tag:
                    break;
                case GyroscopeConfig_mpuI2c_tag:
                    break;
            }
            if (gyroscope != nullptr) gyroscope->initialize();
        }

        for (int i = 0; i < hardwareConfiguration.motors_count; i++) {
            MotorOutput* motorOutput = nullptr;
            switch (hardwareConfiguration.motors[i].motorProtocol) {
                case MotorConfig_MotorProtocol_PWM:
                    break;
                case MotorConfig_MotorProtocol_DShot:
                    break;
            }
            if (motorOutput != nullptr) motors.push_back(motorOutput);
        }

        for (int i = 0; i < hardwareConfiguration.servos_count; i++) {
            ServoOutput* servoOutput = nullptr;
            if (servoOutput != nullptr) servos.push_back(servoOutput);
        }
    }
}
