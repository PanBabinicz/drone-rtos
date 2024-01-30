#ifndef INC_I2C_CONTROLLER_H
#define INC_I2C_CONTROLLER_H

#include "common-defines.h"

#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/gpio.h>

typedef enum {
  I2C_CONTROLLER_SUCCESS = 0,
  I2C_CONTROLLER_ERROR,
  I2C_CONTROLLER_START_ERROR,
  I2C_CONTROLLER_BUSY,
} i2c_controller_status_t;

typedef struct {
  uint32_t  i2c;
  
  /* For standard mode:
   * thigh  = tr(scl) + tw(sclh);
   * tlow   = tr(scl) + tw(scll);
   *
   * tr(scl)  = 1000ns;
   * tw(sclh) = 4000ns;
   * tw(scll) = 4700ns;
   *
   * thigh  = 1000ns + 4000ns = 5000ns;
   * tlow   = 1000ns + 4700ns = 5700ns;
   *
   * thigh  = CCR * Tpclk => CCR = thigh / Tpclk;
   * tlow   = CCR * Tpclk => CCR = thigh / Tpclk;
   *
   * freqeuncy = 36MHz => Tpclk = 28ns;
   *
   * CCR = 5000ns / 28ns => CCR = 178 (0xB2)
   * CCR = 5700ns / 28ns => CCR = 203 (0xCB)
   * */
  uint16_t  ccr;

  /* For standard mode:
   * The maximum allowed SCL rise time is equal 1000ns;
   * frequency = 36MHz => Tpclk = 28ns;
   * 1000ns/28ns = 36 => trise = 36 + 1;
   * */
  uint16_t  trise;

  /* The peripheral clock frequency: 2MHz to 36MHz (the APB frequency). 
   * */
  uint8_t   clock_freq;
} i2c_controller_config_t;

#define I2C_DEFAULT_SM_CONFIG { \
  .i2c        = I2C1, \
  .ccr        = 0xCB, \
  .trise      = 0x25, \
  .clock_freq = 0x24, \
}

extern const i2c_controller_config_t i2c_default_sm_config;

i2c_controller_status_t i2c_controller_init(const i2c_controller_config_t *config);
i2c_controller_status_t i2c_controller_scan(uint32_t i2c);
i2c_controller_status_t i2c_controller_send_byte(uint32_t i2c, uint8_t address, const uint8_t *data);

#endif // !INC_I2C_CONTROLLER_H
