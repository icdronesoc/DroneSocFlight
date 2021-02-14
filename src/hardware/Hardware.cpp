#include "Hardware.h"
#include "config/Config.h"
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
                case AccelerometerConfig_mpuI2c_tag:
                    accelerometer = new AccelerometerDrivers::MpuI2cAccelerometer();
                    break;
                case AccelerometerConfig_mpuSpi_tag:
                    accelerometer = new AccelerometerDrivers::MpuSpiAccelerometer();
                    break;
            }
            if (accelerometer != nullptr) accelerometer->initialize();
        }

        if (Config::hardwareConfig.has_gyroscopeConfig) {
            switch(Config::hardwareConfig.gyroscopeConfig.which_driverConfig) {
                case GyroscopeConfig_mpuI2c_tag:
                    gyroscope = new GyroscopeDrivers::MpuI2cGyroscope();
                    break;
                case GyroscopeConfig_mpuSpi_tag:
                    gyroscope = new GyroscopeDrivers::MpuSpiGyroscope();
                    break;
            }
            if (gyroscope != nullptr) gyroscope->initialize();
        }

        for (int i = 0; i < Config::hardwareConfig.motors_count; i++) {
            Motor* motorOutput = nullptr;
            switch (Config::hardwareConfig.motors[i].motorProtocol) {
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

        for (int i = 0; i < Config::hardwareConfig.servos_count; i++) {
            Servo* servoOutput = new ServoDrivers::PwmServo();
            servos.push_back(servoOutput);
        }
    }
}
