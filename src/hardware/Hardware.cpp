#include "Hardware.h"
#include "IO.h"
#include "hardware/drivers/AllDrivers.h"
#include "log/Log.h"

namespace Hardware {
    namespace { // private
        const char* LogTag = "Hardware";
    }

    Accelerometer* accelerometer = nullptr;
    Gyroscope* gyroscope = nullptr;
    etl::vector<Motor*, maxMotorCount> motors;
    etl::vector<Servo*, maxServoCount> servos;

    void initialize() {
        if (Config::config.has_accelerometerConfig) {
            switch(Config::config.accelerometerConfig.which_driverConfig) {
                case AccelerometerConfig_mpuI2c_tag: {
                    auto index = Config::config.accelerometerConfig.driverConfig.mpuI2c.busIndex;
                    auto address = Config::config.accelerometerConfig.driverConfig.mpuI2c.address;
                    if (index < IO::I2Cs.size() && IO::I2Cs[index] != nullptr && address < 128) {
                        accelerometer = new IMUDrivers::MpuImu(*IO::I2Cs[index], address);
                    } else {
                        Log::error(LogTag, "MPU I2C Accelerometer Configuration Invalid.");
                    }
                    break;
                }
                case AccelerometerConfig_mpuSpi_tag: {
                    auto index = Config::config.accelerometerConfig.driverConfig.mpuSpi.busIndex;
                    auto csPin = IO::findPin(Config::config.accelerometerConfig.driverConfig.mpuSpi.csPin.pinName);
                    if (index < IO::SPIs.size() && IO::SPIs[index] != nullptr && Config::config.accelerometerConfig.driverConfig.mpuSpi.has_csPin && csPin != nullptr) {
                        accelerometer = new IMUDrivers::MpuImu(*IO::SPIs[index], csPin->number);
                    } else {
                        Log::error(LogTag, "MPU SPI Accelerometer Configuration Invalid.");
                    }
                    break;
                }
                default:
                    Log::error(LogTag, "Accelerometer Driver Config type invalid.");
                    break;
            }
        } else {
            Log::warning(LogTag, "No Accelerometer Driver configured.");
        }

        // TODO don't create a second instance of MpuImu
        if (Config::config.has_gyroscopeConfig) {
            switch(Config::config.gyroscopeConfig.which_driverConfig) {
                case GyroscopeConfig_mpuI2c_tag: {
                    auto index = Config::config.gyroscopeConfig.driverConfig.mpuI2c.busIndex;
                    auto address = Config::config.gyroscopeConfig.driverConfig.mpuI2c.address;
                    if (index < IO::I2Cs.size() && IO::I2Cs[index] != nullptr && address < 128) {
                        gyroscope = new IMUDrivers::MpuImu(*IO::I2Cs[index], address);
                    } else {
                        Log::error(LogTag, "MPU I2C Gyroscope Configuration Invalid.");
                    }
                    break;
                }
                case GyroscopeConfig_mpuSpi_tag: {
                    auto index = Config::config.gyroscopeConfig.driverConfig.mpuSpi.busIndex;
                    auto csPin = IO::findPin(Config::config.gyroscopeConfig.driverConfig.mpuSpi.csPin.pinName);
                    if (index < IO::SPIs.size() && IO::SPIs[index] != nullptr && Config::config.gyroscopeConfig.driverConfig.mpuSpi.has_csPin && csPin != nullptr) {
                        gyroscope = new IMUDrivers::MpuImu(*IO::SPIs[index], csPin->number);
                    } else {
                        Log::error(LogTag, "MPU SPI Gyroscope Configuration Invalid.");
                    }
                    break;
                }
                default:
                    Log::error(LogTag, "Gyroscope Driver Config type invalid.");
                    break;
            }
        } else {
            Log::error(LogTag, "No Gyroscope Driver configured.");
        }

        for (pb_size_t i = 0; i < Config::config.motors_count; i++) {
            Motor* motor = nullptr;
            if (Config::config.motors[i].has_outputPin) {
                auto pin = IO::findPin(Config::config.motors[i].outputPin.pinName);
                if (pin != nullptr) {
                    switch (Config::config.motors[i].motorProtocol) {
                        case MotorConfig_MotorProtocol_PWM:
                            motor = new MotorDrivers::PwmMotor(pin->number);
                            break;
                        case MotorConfig_MotorProtocol_DShot:
                            motor = new MotorDrivers::DShotMotor();
                            break;
                        default:
                            Log::error(LogTag, "Motor %s motor protocol invalid.", i);
                            break;
                    }
                } else {
                    Log::error(LogTag, "Motor %s pin does not exist.", i);
                }
            } else {
                Log::error(LogTag, "Motor %s has no pin configured.", i);
            }
            // Even if the motor driver was not initialized, we want to fill the hole so that indexing is constant.
            // eg. If the mixer expects a motor to be index 4, we don't want it to move to index 3 because a previous
            // motor failed to initialize.
            motors.push_back(motor);
        }

        for (pb_size_t i = 0; i < Config::config.servos_count; i++) {
            Servo* servo = nullptr;
            if (Config::config.servos[i].has_outputPin) {
                auto pin = IO::findPin(Config::config.servos[i].outputPin.pinName);
                if (pin != nullptr) {
                    uint32_t refreshRate = 50;
                    switch(Config::config.servos[i].refreshRate) {
                        case ServoConfig_ServoRefreshRate__50Hz:
                            refreshRate = 50;
                            break;
                        case ServoConfig_ServoRefreshRate__330Hz:
                            refreshRate = 330;
                            break;
                        default:
                            Log::error(LogTag, "Servo %s refresh rate invalid.", i);
                            break;
                    }
                    servo = new ServoDrivers::PwmServo(pin->number, refreshRate);
                } else {
                    Log::error(LogTag, "Servo %s pin does not exist.", i);
                }
            } else {
                Log::error(LogTag, "Servo %s has no pin configured.", i);
            }
            // Even if the servo driver was not initialized, we want to fill the hole so that indexing is constant.
            // eg. If the mixer expects a servo to be index 4, we don't want it to move to index 3 because a previous
            // servo failed to initialize.
            servos.push_back(servo);
        }

        Log::info(LogTag, "Hardware configured.");
    }
}
