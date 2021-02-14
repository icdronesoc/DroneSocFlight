#include "Hardware.h"
#include "config/Config.h"
#include "McuHardware.h"
#include "hardware/drivers/AllDrivers.h"

namespace Hardware {
    Accelerometer* accelerometer = nullptr;
    Gyroscope* gyroscope = nullptr;
    etl::vector<Motor*, maxMotorCount> motors;
    etl::vector<Servo*, maxServoCount> servos;

    void initialize() {
        if (Config::hardwareConfiguration.has_busConfig) {
            for (size_t i = 0; i < McuHardware::i2cCount; i++) {
                switch (Config::hardwareConfiguration.busConfig.i2cSpeed) {
                    case BusConfig_I2CSpeed__100KHz:
                        McuHardware::I2Cs[i].setClock(100000);
                        break;
                }
            }
        }

        if (Config::hardwareConfiguration.has_accelerometerConfig) {
            switch(Config::hardwareConfiguration.accelerometerConfig.which_driverConfig) {
                case AccelerometerConfig_mpuI2c_tag:
                    accelerometer = new AccelerometerDrivers::MpuI2cAccelerometer();
                    break;
                case AccelerometerConfig_mpuSpi_tag:
                    accelerometer = new AccelerometerDrivers::MpuSpiAccelerometer();
                    break;
            }
            if (accelerometer != nullptr) accelerometer->initialize();
        }

        if (Config::hardwareConfiguration.has_gyroscopeConfig) {
            switch(Config::hardwareConfiguration.gyroscopeConfig.which_driverConfig) {
                case GyroscopeConfig_mpuI2c_tag:
                    gyroscope = new GyroscopeDrivers::MpuI2cGyroscope();
                    break;
                case GyroscopeConfig_mpuSpi_tag:
                    gyroscope = new GyroscopeDrivers::MpuSpiGyroscope();
                    break;
            }
            if (gyroscope != nullptr) gyroscope->initialize();
        }

        for (int i = 0; i < Config::hardwareConfiguration.motors_count; i++) {
            Motor* motorOutput = nullptr;
            switch (Config::hardwareConfiguration.motors[i].motorProtocol) {
                case MotorConfig_MotorProtocol_PWM:
                    motorOutput = new MotorDrivers::PwmMotor();
                    break;
                case MotorConfig_MotorProtocol_DShot:
                    motorOutput = new MotorDrivers::DShotMotor();
                    break;
            }
            // Even if the motor driver was not initialized, we want to fill the hole so that indexing is constant.
            // eg. If the mixer expects a motor to be index 4, we don't want it to move to index 3 because a previous
            // motor failed to initialize.
            motors.push_back(motorOutput);
        }

        for (int i = 0; i < Config::hardwareConfiguration.servos_count; i++) {
            Servo* servoOutput = new ServoDrivers::PwmServo();
            servos.push_back(servoOutput);
        }
    }
}
