#include "Hardware.h"
#include "IO.h"
#include "hardware/drivers/AllDrivers.h"

namespace Hardware {
    Accelerometer* accelerometer = nullptr;
    Gyroscope* gyroscope = nullptr;
    etl::vector<Motor*, maxMotorCount> motors;
    etl::vector<Servo*, maxServoCount> servos;

    void initialize() {
        if (Config::hardwareConfig.has_accelerometerConfig) {
            switch(Config::hardwareConfig.accelerometerConfig.which_driverConfig) {
                case AccelerometerConfig_mpuI2c_tag: {
                    auto index = Config::hardwareConfig.accelerometerConfig.driverConfig.mpuI2c.busIndex;
                    auto address = Config::hardwareConfig.accelerometerConfig.driverConfig.mpuI2c.address;
                    if (index < IO::I2Cs.size() && address < 128) {
                        accelerometer = new IMUDrivers::MpuImu(IO::I2Cs[index], address);
                    }
                    break;
                }
                case AccelerometerConfig_mpuSpi_tag: {
                    auto index = Config::hardwareConfig.accelerometerConfig.driverConfig.mpuSpi.busIndex;
                    auto csPin = IO::findPin(Config::hardwareConfig.accelerometerConfig.driverConfig.mpuSpi.csPin.pinName);
                    if (index < IO::SPIs.size() && Config::hardwareConfig.accelerometerConfig.driverConfig.mpuSpi.has_csPin && csPin != nullptr) {
                        accelerometer = new IMUDrivers::MpuImu(IO::SPIs[index], csPin->number);
                    }
                    break;
                }
            }
        }

        // TODO don't create a second instance of MpuImu
        if (Config::hardwareConfig.has_gyroscopeConfig) {
            switch(Config::hardwareConfig.gyroscopeConfig.which_driverConfig) {
                case GyroscopeConfig_mpuI2c_tag: {
                    auto index = Config::hardwareConfig.gyroscopeConfig.driverConfig.mpuI2c.busIndex;
                    auto address = Config::hardwareConfig.gyroscopeConfig.driverConfig.mpuI2c.address;
                    if (index < IO::I2Cs.size() && address < 128) {
                        gyroscope = new IMUDrivers::MpuImu(IO::I2Cs[index], address);
                    }
                    break;
                }
                case GyroscopeConfig_mpuSpi_tag: {
                    auto index = Config::hardwareConfig.gyroscopeConfig.driverConfig.mpuSpi.busIndex;
                    auto csPin = IO::findPin(Config::hardwareConfig.gyroscopeConfig.driverConfig.mpuSpi.csPin.pinName);
                    if (index < IO::SPIs.size() && Config::hardwareConfig.gyroscopeConfig.driverConfig.mpuSpi.has_csPin && csPin != nullptr) {
                        gyroscope = new IMUDrivers::MpuImu(IO::SPIs[index], csPin->number);
                    }
                    break;
                }
            }
        }

        for (pb_size_t i = 0; i < Config::hardwareConfig.motors_count; i++) {
            Motor* motor = nullptr;
            if (Config::hardwareConfig.motors[i].has_outputPin) {
                auto pin = IO::findPin(Config::hardwareConfig.motors[i].outputPin.pinName);
                if (pin != nullptr) {
                    switch (Config::hardwareConfig.motors[i].motorProtocol) {
                        case MotorConfig_MotorProtocol_PWM:
                            motor = new MotorDrivers::PwmMotor(pin->number);
                            break;
                        case MotorConfig_MotorProtocol_DShot:
                            motor = new MotorDrivers::DShotMotor();
                            break;
                    }
                }
            }
            // Even if the motor driver was not initialized, we want to fill the hole so that indexing is constant.
            // eg. If the mixer expects a motor to be index 4, we don't want it to move to index 3 because a previous
            // motor failed to initialize.
            motors.push_back(motor);
        }

        for (pb_size_t i = 0; i < Config::hardwareConfig.servos_count; i++) {
            Servo* servo = nullptr;
            if (Config::hardwareConfig.servos[i].has_outputPin) {
                auto pin = IO::findPin(Config::hardwareConfig.servos[i].outputPin.pinName);
                if (pin != nullptr) {
                    uint32_t refreshRate = 50;
                    switch(Config::hardwareConfig.servos[i].refreshRate) {
                        case ServoConfig_ServoRefreshRate__50Hz:
                            refreshRate = 50;
                            break;
                        case ServoConfig_ServoRefreshRate__330Hz:
                            refreshRate = 330;
                            break;
                    }
                    servo = new ServoDrivers::PwmServo(pin->number, refreshRate);
                }
            }
            // Even if the servo driver was not initialized, we want to fill the hole so that indexing is constant.
            // eg. If the mixer expects a servo to be index 4, we don't want it to move to index 3 because a previous
            // servo failed to initialize.
            servos.push_back(servo);
        }
    }
}
