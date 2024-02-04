#include "i2c_controller.h"
#include <mpu6050.h>

mpu6050_status_t mpu6050_whoami(uint32_t i2c, uint8_t *data) {
  if (!data) {
    return MPU6050_ERROR;
  }

  if (i2c_controller_read_instruction(i2c, MPU6050_ADDRESS, MPU6050_WHO_AM_I, data, sizeof(data))) {
    return MPU6050_ERROR;
  }

  return MPU6050_SUCCESS;
}

mpu6050_status_t mpu6050_reset(uint32_t i2c) {
  /* Device reset Power Management 1 register. */
  if (i2c_controller_send_instruction(i2c, MPU6050_ADDRESS, MPU6050_DEVICE_RESET_INSTR, 
                                      MPU6050_INSTR_SIZE(MPU6050_DEVICE_RESET_INSTR))) {
    return MPU6050_ERROR;
  }

  /* Signal path reset. */
  if (i2c_controller_send_instruction(i2c, MPU6050_ADDRESS, MPU6050_SIGNAL_PATH_RESET_INSTR, 
                                      MPU6050_INSTR_SIZE(MPU6050_SIGNAL_PATH_RESET_INSTR))) {
    return MPU6050_ERROR;
  }

  return MPU6050_SUCCESS;
}

mpu6050_status_t mpu6050_sleep_off(uint32_t i2c) {
  /* CLKSEL bit selection. Turn off sleep mode. */
  if (i2c_controller_send_instruction(i2c, MPU6050_ADDRESS, MPU6050_CLKSEL_INSTR, 
                                      MPU6050_INSTR_SIZE(MPU6050_CLKSEL_INSTR))) {
    return MPU6050_ERROR;
  }

  return MPU6050_SUCCESS;
}

mpu6050_status_t mpu6050_config(uint32_t i2c) {
  /* GYRO Configuration. */
  if (i2c_controller_send_instruction(i2c, MPU6050_ADDRESS, MPU6050_CONFIGURE_GYRO_INSTR, 
                                      MPU6050_INSTR_SIZE(MPU6050_CONFIGURE_GYRO_INSTR))) {
    return MPU6050_ERROR;
  }

  /* ACCEL Configuration. */
  if (i2c_controller_send_instruction(i2c, MPU6050_ADDRESS, MPU6050_CONFIGURE_ACCEL_INSTR, 
                                      MPU6050_INSTR_SIZE(MPU6050_CONFIGURE_ACCEL_INSTR))) {
    return MPU6050_ERROR;
  }

  return MPU6050_SUCCESS;
}

mpu6050_status_t mpu6050_get_pwr_mgmt_1(uint32_t i2c) {
  uint8_t config_data;

  i2c_controller_read_instruction(i2c, MPU6050_ADDRESS, MPU6050_PWR_MGMT_1, &config_data, sizeof(config_data));

  return MPU6050_SUCCESS;
}

mpu6050_status_t mpu6050_get_accel(uint32_t i2c, mpu6050_accel_data_t *accel) {
  if (!accel)  {
    return MPU6050_ERROR;
  }

  uint8_t accel_data[6];

  i2c_controller_read_instruction(i2c, MPU6050_ADDRESS, MPU6050_ACCEL_XOUT_H, accel_data, sizeof(accel_data));

  accel->x = (((uint16_t)accel_data[0] << 8) | ((uint16_t)accel_data[1]));
  accel->y = (((uint16_t)accel_data[2] << 8) | ((uint16_t)accel_data[3]));
  accel->z = (((uint16_t)accel_data[4] << 8) | ((uint16_t)accel_data[5]));

  return MPU6050_SUCCESS;
}

mpu6050_status_t mpu6050_get_gyro(uint32_t i2c, mpu6050_gyro_data_t *gyro) {
  if (!gyro)  {
    return MPU6050_ERROR;
  }

  uint8_t gyro_data[6];

  if (i2c_controller_read_instruction(i2c, MPU6050_ADDRESS, MPU6050_GYRO_XOUT_H, gyro_data, sizeof(gyro_data))) {
    return MPU6050_ERROR;
  }

  gyro->x = (((uint16_t)gyro_data[0] << 8) | ((uint16_t)gyro_data[1]));
  gyro->y = (((uint16_t)gyro_data[2] << 8) | ((uint16_t)gyro_data[3]));
  gyro->z = (((uint16_t)gyro_data[4] << 8) | ((uint16_t)gyro_data[5]));

  return MPU6050_SUCCESS;
}

mpu6050_status_t mpu6050_get_temp(uint32_t i2c, mpu6050_temp_data_t *temp) {
  if (!temp) {
    return MPU6050_ERROR;
  }

  uint8_t temp_data[2];

  if (i2c_controller_read_instruction(i2c, MPU6050_ADDRESS, MPU6050_TEMP_OUT_H, temp_data, sizeof(temp_data))) {
    return MPU6050_ERROR;
  }

  temp->raw_temp = (((uint16_t)temp_data[0] << 8) | (uint16_t)temp_data[1]);

  return MPU6050_SUCCESS;
}

mpu6050_status_t mpu6050_get_all_measurements(uint32_t i2c, mpu6050_accel_data_t *accel, mpu6050_gyro_data_t *gyro, mpu6050_temp_data_t *temp) {
  if ((!accel) || (!gyro) || (!temp)) {
    return MPU6050_ERROR;
  }

  /* 6 bytes for accel; 6 bytes for gyro; 2 bytes for temp */
  uint8_t data[14];

  if (i2c_controller_read_instruction(i2c, MPU6050_ADDRESS, MPU6050_ACCEL_XOUT_H, data, sizeof(data))) {
    return MPU6050_ERROR;
  }

  accel->x = (((uint16_t)data[0] << 8) | ((uint16_t)data[1]));
  accel->y = (((uint16_t)data[2] << 8) | ((uint16_t)data[3]));
  accel->z = (((uint16_t)data[4] << 8) | ((uint16_t)data[5]));

  gyro->x = (((uint16_t)data[6] << 8) | ((uint16_t)data[7]));
  gyro->y = (((uint16_t)data[8] << 8) | ((uint16_t)data[9]));
  gyro->z = (((uint16_t)data[10] << 8) | ((uint16_t)data[11]));

  temp->raw_temp = (((uint16_t)data[12] << 8) | (uint16_t)data[13]);

  return MPU6050_SUCCESS;
}
