#include <i2c_controller.h>
#include <libopencm3/stm32/gpio.h>

static void delay(uint32_t cycles) {
  for (uint32_t i = 0; i < cycles; i++) {
    __asm__("nop");
  }
}

i2c_controller_status_t i2c_controller_send(uint32_t i2c, uint8_t *data) {
  if (!data) {
    for (uint8_t i = 0; i < 3; i++) {
      gpio_clear(GPIOC, GPIO13); 
      delay(RCC_CLOCK_SPEED_HZ/16);
    }
    return I2C_CONTROLLER_ERROR;
  }

  uint32_t counter; 

  /* Wait until the bus become free */
  counter = RCC_CLOCK_SPEED_HZ / 4;
  while ((I2C_SR2(i2c) & (I2C_SR2_BUSY))) {
    counter--;
    if (!counter) {
      return I2C_CONTROLLER_BUSY;
    }
  }

  /* A serial data transfer always begins with a start condition 
   * and ends with a stop condition */
  i2c_send_start(i2c);

  /* Setting the START bit causes the interface to generate a Start condition and to switch 
   * to Master mode (MSL bit set) when the BUSY bit is cleared. */
  counter = RCC_CLOCK_SPEED_HZ / 4;
  while (!(I2C_SR1(i2c) & I2C_SR1_SB) && (I2C_SR2(i2c) & (I2C_SR2_MSL | I2C_SR2_BUSY))) {
    counter--;
    if (!counter) {
      
      
      return I2C_CONTROLLER_START_ERROR;
    }
  }

  i2c_send_7bit_address(i2c, 0x55, I2C_WRITE);

  i2c_send_stop(i2c);

  return I2C_CONTROLLER_SUCCESS;
} 
