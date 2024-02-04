#ifndef INC_MPU6050_H
#define INC_MPU6050_H

#include <common-defines.h>
#include <i2c_controller.h>

/* MPU6050 Address: AD0 pin is conntected to Vcc */
#define MPU6050_ADDRESS (0x69)

/* MPU6050 Registers. */
#define MPU6050_SELF_TEST_X         (0x0D)
#define MPU6050_SELF_TEST_Y         (0x0E)
#define MPU6050_SELF_TEST_Z         (0x0F)
#define MPU6050_SELF_TEST_A         (0x10)
#define MPU6050_SMPLRT_DIV          (0x19)
#define MPU6050_CONFIG              (0x1A)
#define MPU6050_GYRO_CONFIG         (0x1B)
#define MPU6050_ACCEL_CONFIG        (0x1C)
#define MPU6050_FIFO_EN             (0x23)
#define MPU6050_I2C_MST_CTRL        (0x24)
#define MPU6050_I2C_SLV0_ADDR       (0x25)
#define MPU6050_I2C_SLV0_REG        (0x26)
#define MPU6050_I2C_SLV0_CTRL       (0x27)
#define MPU6050_I2C_SLV1_ADDR       (0x28)
#define MPU6050_I2C_SLV1_REG        (0x29)
#define MPU6050_I2C_SLV1_CTRL       (0x2A)
#define MPU6050_I2C_SLV2_ADDR       (0x2B)
#define MPU6050_I2C_SLV2_REG        (0x2C)
#define MPU6050_I2C_SLV2_CTRL       (0x2D)
#define MPU6050_I2C_SLV3_ADDR       (0x2E)
#define MPU6050_I2C_SLV3_REG        (0x2F)
#define MPU6050_I2C_SLV3_CTRL       (0x30)
#define MPU6050_I2C_SLV4_ADDR       (0x31)
#define MPU6050_I2C_SLV4_REG        (0x32)
#define MPU6050_I2C_SLV4_DO         (0x33)
#define MPU6050_I2C_SLV4_CTRL       (0x34)
#define MPU6050_I2C_SLV4_DI         (0x35)
#define MPU6050_I2C_MST_STATUS      (0x36)
#define MPU6050_INT_PIN_CFG         (0x37)
#define MPU6050_INT_ENABLE          (0x38)
#define MPU6050_INT_STATUS          (0x3A)
#define MPU6050_ACCEL_XOUT_H        (0x3B)
#define MPU6050_ACCEL_XOUT_L        (0x3C)
#define MPU6050_ACCEL_YOUT_H        (0x3D)
#define MPU6050_ACCEL_YOUT_L        (0x3E)
#define MPU6050_ACCEL_ZOUT_H        (0x3F)
#define MPU6050_ACCEL_ZOUT_L        (0x40)
#define MPU6050_TEMP_OUT_H          (0x41)
#define MPU6050_TEMP_OUT_L          (0x42)
#define MPU6050_GYRO_XOUT_H         (0x43)
#define MPU6050_GYRO_XOUT_L         (0x44)
#define MPU6050_GYRO_YOUT_H         (0x45)
#define MPU6050_GYRO_YOUT_L         (0x46)
#define MPU6050_GYRO_ZOUT_H         (0x47)
#define MPU6050_GYRO_ZOUT_L         (0x48)
#define MPU6050_EXT_SENS_DATA_00    (0x49)
#define MPU6050_EXT_SENS_DATA_01    (0x4A)
#define MPU6050_EXT_SENS_DATA_02    (0x4B)
#define MPU6050_EXT_SENS_DATA_03    (0x4C)
#define MPU6050_EXT_SENS_DATA_04    (0x4D)
#define MPU6050_EXT_SENS_DATA_05    (0x4E)
#define MPU6050_EXT_SENS_DATA_06    (0x4F)
#define MPU6050_EXT_SENS_DATA_07    (0x50)
#define MPU6050_EXT_SENS_DATA_08    (0x51)
#define MPU6050_EXT_SENS_DATA_09    (0x52)
#define MPU6050_EXT_SENS_DATA_10    (0x53)
#define MPU6050_EXT_SENS_DATA_11    (0x54)
#define MPU6050_EXT_SENS_DATA_12    (0x55)
#define MPU6050_EXT_SENS_DATA_13    (0x56)
#define MPU6050_EXT_SENS_DATA_14    (0x57)
#define MPU6050_EXT_SENS_DATA_15    (0x58)
#define MPU6050_EXT_SENS_DATA_16    (0x59)
#define MPU6050_EXT_SENS_DATA_17    (0x5A)
#define MPU6050_EXT_SENS_DATA_18    (0x5B)
#define MPU6050_EXT_SENS_DATA_19    (0x5C)
#define MPU6050_EXT_SENS_DATA_20    (0x5D)
#define MPU6050_EXT_SENS_DATA_21    (0x5E)
#define MPU6050_EXT_SENS_DATA_22    (0x5F)
#define MPU6050_EXT_SENS_DATA_23    (0x60)
#define MPU6050_I2C_SLV0_DO         (0x63)
#define MPU6050_I2C_SLV1_DO         (0x64)
#define MPU6050_I2C_SLV2_DO         (0x65)
#define MPU6050_I2C_SLV3_DO         (0x66)
#define MPU6050_I2C_MST_DELAY_CTRL  (0x67)
#define MPU6050_SIGNAL_PATH_RESET   (0x68)
#define MPU6050_USER_CTRL           (0x6A)
#define MPU6050_PWR_MGMT_1          (0x6B)
#define MPU6050_PWR_MGMT_2          (0x6C)
#define MPU6050_FIFO_COUNTH         (0x72)
#define MPU6050_FIFO_COUNTL         (0x73)
#define MPU6050_FIFO_R_W            (0x74)
#define MPU6050_WHO_AM_I            (0x75)

/* Signal path register bits. */
#define MPU6050_TEMP_RESET  (1 << 0)
#define MPU6050_ACCEL_RESET (1 << 1)
#define MPU6050_GYRO_RESET  (1 << 2)

/* GYRO Config definitions. */
#define MPU6050_XG_ST (1 << 7)
#define MPU6050_YG_ST (1 << 5)
#define MPU6050_ZG_ST (1 << 4)

#define MPU6050_FS_SEL_250  (~(3 << 3))
#define MPU6050_FS_SEL_500  (1 << 3)
#define MPU6050_FS_SEL_1000 (2 << 3)
#define MPU6050_FS_SEL_2000 (3 << 3)

/* ACCEL Config definitions. */
#define MPU6050_XA_ST (1 << 7)
#define MPU6050_YA_ST (1 << 5)
#define MPU6050_ZA_ST (1 << 4)

#define MPU6050_AFS_SEL_2   (~(3 << 3))
#define MPU6050_AFS_SEL_4   (1 << 3)
#define MPU6050_AFS_SEL_8   (2 << 3)
#define MPU6050_AFS_SEL_16  (3 << 3)

/* Power Management 1 register bits. */
#define MPU6050_DEVICE_RESET_BIT  (1 << 7)

#define MPU6050_CLKSEL_INTERNAL   (0 << 0)
#define MPU6050_CLKSEL_PLL_XGYRO  (1 << 0)
#define MPU6050_CLKSEL_PLL_YGYRO  (2 << 0)
#define MPU6050_CLKSEL_PLL_ZGYRO  (3 << 0)
#define MPU6050_CLKSEL_PLL_EXT32  (4 << 0)
#define MPU6050_CLKSEL_PLL_EXT19  (5 << 0)
#define MPU6050_CLKSEL_STOP       (7 << 0)


/* MPU6050 instructions. */
#define MPU6050_CLKSEL_INSTR            ((uint8_t[]){ (uint8_t)MPU6050_PWR_MGMT_1, (uint8_t)(MPU6050_CLKSEL_PLL_EXT32) })
#define MPU6050_DEVICE_RESET_INSTR      ((uint8_t[]){ (uint8_t)MPU6050_PWR_MGMT_1, (uint8_t)(MPU6050_DEVICE_RESET_BIT) })
#define MPU6050_SIGNAL_PATH_RESET_INSTR ((uint8_t[]){ (uint8_t)MPU6050_SIGNAL_PATH_RESET, (uint8_t)(MPU6050_TEMP_RESET | MPU6050_ACCEL_RESET | MPU6050_GYRO_RESET) })
#define MPU6050_CONFIGURE_GYRO_INSTR    ((uint8_t[]){ (uint8_t)MPU6050_GYRO_CONFIG, (uint8_t)(MPU6050_FS_SEL_2000) }) 
#define MPU6050_CONFIGURE_ACCEL_INSTR   ((uint8_t[]){ (uint8_t)MPU6050_ACCEL_CONFIG, (uint8_t)(MPU6050_AFS_SEL_16) }) 

#define MPU6050_INSTR_SIZE(INSTR) (sizeof(INSTR))

typedef enum {
  MPU6050_SUCCESS = 0,
  MPU6050_ERROR,
} mpu6050_status_t;

typedef struct {
  uint16_t x;
  uint16_t y;
  uint16_t z;
} mpu6050_gyro_data_t;

typedef struct {
  uint16_t x;
  uint16_t y;
  uint16_t z;
} mpu6050_accel_data_t;

typedef struct {
  uint16_t raw_temp;
} mpu6050_temp_data_t;

mpu6050_status_t mpu6050_whoami(uint32_t i2c, uint8_t *data);
mpu6050_status_t mpu6050_reset(uint32_t i2c);
mpu6050_status_t mpu6050_sleep_off(uint32_t i2c);
mpu6050_status_t mpu6050_config(uint32_t i2c);
mpu6050_status_t mpu6050_get_pwr_mgmt_1(uint32_t i2c);
mpu6050_status_t mpu6050_get_accel(uint32_t i2c, mpu6050_accel_data_t *accel);
mpu6050_status_t mpu6050_get_temp(uint32_t i2c, mpu6050_temp_data_t *temp);
mpu6050_status_t mpu6050_get_gyro(uint32_t i2c, mpu6050_gyro_data_t *gyro);
mpu6050_status_t mpu6050_get_all_measurements(uint32_t i2c, mpu6050_accel_data_t *accel, mpu6050_gyro_data_t *gyro, mpu6050_temp_data_t *temp);

#endif // !INC_MPU6050_H
