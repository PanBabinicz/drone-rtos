#ifndef INC_I2C_CONTROLLER_H
#define INC_I2C_CONTROLLER_H

#include "common-defines.h"

#include <libopencm3/stm32/i2c.h>

typedef enum {
  I2C_CONTROLLER_SUCCESS = 0,
  I2C_CONTROLLER_ERROR,
  I2C_CONTROLLER_START_ERROR,
  I2C_CONTROLLER_BUSY,
} i2c_controller_status_t;

i2c_controller_status_t i2c_controller_send(uint32_t i2c, uint8_t *data);

#endif // !INC_I2C_CONTROLLER_H
