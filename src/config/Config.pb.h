/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.2-dev */

#ifndef PB_CONFIG_CONFIG_PB_H_INCLUDED
#define PB_CONFIG_CONFIG_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Enum definitions */
typedef enum _BusConfig_I2CSpeed {
    BusConfig_I2CSpeed__100KHz = 0
} BusConfig_I2CSpeed;

typedef enum _MotorConfig_MotorProtocol {
    MotorConfig_MotorProtocol_PWM = 0,
    MotorConfig_MotorProtocol_DShot = 1
} MotorConfig_MotorProtocol;

typedef enum _ServoConfig_ServoRefreshRate {
    ServoConfig_ServoRefreshRate__50Hz = 0,
    ServoConfig_ServoRefreshRate__330Hz = 1
} ServoConfig_ServoRefreshRate;

/* Struct definitions */
typedef struct _SoftwareConfiguration {
    char dummy_field;
} SoftwareConfiguration;

typedef struct _BusConfig {
    BusConfig_I2CSpeed i2cSpeed;
} BusConfig;

typedef struct _MotorConfig {
    uint32_t outputPin;
    MotorConfig_MotorProtocol motorProtocol;
} MotorConfig;

typedef struct _MpuI2cConfig {
    uint32_t busIndex;
    uint32_t address;
} MpuI2cConfig;

typedef struct _MpuSpiConfig {
    uint32_t busIndex;
    uint32_t csPin;
} MpuSpiConfig;

typedef struct _ServoConfig {
    uint32_t outputPin;
    ServoConfig_ServoRefreshRate refreshRate;
} ServoConfig;

typedef struct _AccelerometerConfig {
    pb_size_t which_driverConfig;
    union {
        MpuI2cConfig mpuI2c;
        MpuSpiConfig mpuSpi;
    } driverConfig;
} AccelerometerConfig;

typedef struct _GyroscopeConfig {
    pb_size_t which_driverConfig;
    union {
        MpuI2cConfig mpuI2c;
        MpuSpiConfig mpuSpi;
    } driverConfig;
} GyroscopeConfig;

typedef struct _HardwareConfiguration {
    bool has_busConfig;
    BusConfig busConfig;
    bool has_accelerometerConfig;
    AccelerometerConfig accelerometerConfig;
    bool has_gyroscopeConfig;
    GyroscopeConfig gyroscopeConfig;
    pb_size_t motors_count;
    MotorConfig motors[8];
    pb_size_t servos_count;
    ServoConfig servos[8];
} HardwareConfiguration;


/* Helper constants for enums */
#define _BusConfig_I2CSpeed_MIN BusConfig_I2CSpeed__100KHz
#define _BusConfig_I2CSpeed_MAX BusConfig_I2CSpeed__100KHz
#define _BusConfig_I2CSpeed_ARRAYSIZE ((BusConfig_I2CSpeed)(BusConfig_I2CSpeed__100KHz+1))

#define _MotorConfig_MotorProtocol_MIN MotorConfig_MotorProtocol_PWM
#define _MotorConfig_MotorProtocol_MAX MotorConfig_MotorProtocol_DShot
#define _MotorConfig_MotorProtocol_ARRAYSIZE ((MotorConfig_MotorProtocol)(MotorConfig_MotorProtocol_DShot+1))

#define _ServoConfig_ServoRefreshRate_MIN ServoConfig_ServoRefreshRate__50Hz
#define _ServoConfig_ServoRefreshRate_MAX ServoConfig_ServoRefreshRate__330Hz
#define _ServoConfig_ServoRefreshRate_ARRAYSIZE ((ServoConfig_ServoRefreshRate)(ServoConfig_ServoRefreshRate__330Hz+1))


/* Initializer values for message structs */
#define HardwareConfiguration_init_default       {false, BusConfig_init_default, false, AccelerometerConfig_init_default, false, GyroscopeConfig_init_default, 0, {MotorConfig_init_default, MotorConfig_init_default, MotorConfig_init_default, MotorConfig_init_default, MotorConfig_init_default, MotorConfig_init_default, MotorConfig_init_default, MotorConfig_init_default}, 0, {ServoConfig_init_default, ServoConfig_init_default, ServoConfig_init_default, ServoConfig_init_default, ServoConfig_init_default, ServoConfig_init_default, ServoConfig_init_default, ServoConfig_init_default}}
#define SoftwareConfiguration_init_default       {0}
#define BusConfig_init_default                   {_BusConfig_I2CSpeed_MIN}
#define AccelerometerConfig_init_default         {0, {MpuI2cConfig_init_default}}
#define GyroscopeConfig_init_default             {0, {MpuI2cConfig_init_default}}
#define MpuSpiConfig_init_default                {0, 0}
#define MpuI2cConfig_init_default                {0, 0}
#define MotorConfig_init_default                 {0, _MotorConfig_MotorProtocol_MIN}
#define ServoConfig_init_default                 {0, _ServoConfig_ServoRefreshRate_MIN}
#define HardwareConfiguration_init_zero          {false, BusConfig_init_zero, false, AccelerometerConfig_init_zero, false, GyroscopeConfig_init_zero, 0, {MotorConfig_init_zero, MotorConfig_init_zero, MotorConfig_init_zero, MotorConfig_init_zero, MotorConfig_init_zero, MotorConfig_init_zero, MotorConfig_init_zero, MotorConfig_init_zero}, 0, {ServoConfig_init_zero, ServoConfig_init_zero, ServoConfig_init_zero, ServoConfig_init_zero, ServoConfig_init_zero, ServoConfig_init_zero, ServoConfig_init_zero, ServoConfig_init_zero}}
#define SoftwareConfiguration_init_zero          {0}
#define BusConfig_init_zero                      {_BusConfig_I2CSpeed_MIN}
#define AccelerometerConfig_init_zero            {0, {MpuI2cConfig_init_zero}}
#define GyroscopeConfig_init_zero                {0, {MpuI2cConfig_init_zero}}
#define MpuSpiConfig_init_zero                   {0, 0}
#define MpuI2cConfig_init_zero                   {0, 0}
#define MotorConfig_init_zero                    {0, _MotorConfig_MotorProtocol_MIN}
#define ServoConfig_init_zero                    {0, _ServoConfig_ServoRefreshRate_MIN}

/* Field tags (for use in manual encoding/decoding) */
#define BusConfig_i2cSpeed_tag                   1
#define MotorConfig_outputPin_tag                1
#define MotorConfig_motorProtocol_tag            2
#define MpuI2cConfig_busIndex_tag                1
#define MpuI2cConfig_address_tag                 2
#define MpuSpiConfig_busIndex_tag                1
#define MpuSpiConfig_csPin_tag                   2
#define ServoConfig_outputPin_tag                1
#define ServoConfig_refreshRate_tag              2
#define AccelerometerConfig_mpuI2c_tag           1
#define AccelerometerConfig_mpuSpi_tag           2
#define GyroscopeConfig_mpuI2c_tag               1
#define GyroscopeConfig_mpuSpi_tag               2
#define HardwareConfiguration_busConfig_tag      1
#define HardwareConfiguration_accelerometerConfig_tag 2
#define HardwareConfiguration_gyroscopeConfig_tag 3
#define HardwareConfiguration_motors_tag         4
#define HardwareConfiguration_servos_tag         5

/* Struct field encoding specification for nanopb */
#define HardwareConfiguration_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, MESSAGE,  busConfig,         1) \
X(a, STATIC,   OPTIONAL, MESSAGE,  accelerometerConfig,   2) \
X(a, STATIC,   OPTIONAL, MESSAGE,  gyroscopeConfig,   3) \
X(a, STATIC,   REPEATED, MESSAGE,  motors,            4) \
X(a, STATIC,   REPEATED, MESSAGE,  servos,            5)
#define HardwareConfiguration_CALLBACK NULL
#define HardwareConfiguration_DEFAULT NULL
#define HardwareConfiguration_busConfig_MSGTYPE BusConfig
#define HardwareConfiguration_accelerometerConfig_MSGTYPE AccelerometerConfig
#define HardwareConfiguration_gyroscopeConfig_MSGTYPE GyroscopeConfig
#define HardwareConfiguration_motors_MSGTYPE MotorConfig
#define HardwareConfiguration_servos_MSGTYPE ServoConfig

#define SoftwareConfiguration_FIELDLIST(X, a) \

#define SoftwareConfiguration_CALLBACK NULL
#define SoftwareConfiguration_DEFAULT NULL

#define BusConfig_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UENUM,    i2cSpeed,          1)
#define BusConfig_CALLBACK NULL
#define BusConfig_DEFAULT NULL

#define AccelerometerConfig_FIELDLIST(X, a) \
X(a, STATIC,   ONEOF,    MESSAGE,  (driverConfig,mpuI2c,driverConfig.mpuI2c),   1) \
X(a, STATIC,   ONEOF,    MESSAGE,  (driverConfig,mpuSpi,driverConfig.mpuSpi),   2)
#define AccelerometerConfig_CALLBACK NULL
#define AccelerometerConfig_DEFAULT NULL
#define AccelerometerConfig_driverConfig_mpuI2c_MSGTYPE MpuI2cConfig
#define AccelerometerConfig_driverConfig_mpuSpi_MSGTYPE MpuSpiConfig

#define GyroscopeConfig_FIELDLIST(X, a) \
X(a, STATIC,   ONEOF,    MESSAGE,  (driverConfig,mpuI2c,driverConfig.mpuI2c),   1) \
X(a, STATIC,   ONEOF,    MESSAGE,  (driverConfig,mpuSpi,driverConfig.mpuSpi),   2)
#define GyroscopeConfig_CALLBACK NULL
#define GyroscopeConfig_DEFAULT NULL
#define GyroscopeConfig_driverConfig_mpuI2c_MSGTYPE MpuI2cConfig
#define GyroscopeConfig_driverConfig_mpuSpi_MSGTYPE MpuSpiConfig

#define MpuSpiConfig_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   busIndex,          1) \
X(a, STATIC,   SINGULAR, UINT32,   csPin,             2)
#define MpuSpiConfig_CALLBACK NULL
#define MpuSpiConfig_DEFAULT NULL

#define MpuI2cConfig_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   busIndex,          1) \
X(a, STATIC,   SINGULAR, UINT32,   address,           2)
#define MpuI2cConfig_CALLBACK NULL
#define MpuI2cConfig_DEFAULT NULL

#define MotorConfig_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   outputPin,         1) \
X(a, STATIC,   SINGULAR, UENUM,    motorProtocol,     2)
#define MotorConfig_CALLBACK NULL
#define MotorConfig_DEFAULT NULL

#define ServoConfig_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   outputPin,         1) \
X(a, STATIC,   SINGULAR, UENUM,    refreshRate,       2)
#define ServoConfig_CALLBACK NULL
#define ServoConfig_DEFAULT NULL

extern const pb_msgdesc_t HardwareConfiguration_msg;
extern const pb_msgdesc_t SoftwareConfiguration_msg;
extern const pb_msgdesc_t BusConfig_msg;
extern const pb_msgdesc_t AccelerometerConfig_msg;
extern const pb_msgdesc_t GyroscopeConfig_msg;
extern const pb_msgdesc_t MpuSpiConfig_msg;
extern const pb_msgdesc_t MpuI2cConfig_msg;
extern const pb_msgdesc_t MotorConfig_msg;
extern const pb_msgdesc_t ServoConfig_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define HardwareConfiguration_fields &HardwareConfiguration_msg
#define SoftwareConfiguration_fields &SoftwareConfiguration_msg
#define BusConfig_fields &BusConfig_msg
#define AccelerometerConfig_fields &AccelerometerConfig_msg
#define GyroscopeConfig_fields &GyroscopeConfig_msg
#define MpuSpiConfig_fields &MpuSpiConfig_msg
#define MpuI2cConfig_fields &MpuI2cConfig_msg
#define MotorConfig_fields &MotorConfig_msg
#define ServoConfig_fields &ServoConfig_msg

/* Maximum encoded size of messages (where known) */
#define HardwareConfiguration_size               196
#define SoftwareConfiguration_size               0
#define BusConfig_size                           2
#define AccelerometerConfig_size                 14
#define GyroscopeConfig_size                     14
#define MpuSpiConfig_size                        12
#define MpuI2cConfig_size                        12
#define MotorConfig_size                         8
#define ServoConfig_size                         8

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif