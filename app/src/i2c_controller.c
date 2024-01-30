#include <i2c_controller.h>
#include <stdint.h>
#include <usart_controller.h>
#include <libopencm3/stm32/usart.h>
#include <message.h>

uint16_t  data_buffer[128];
char      hex_string[10];

static void delay(uint32_t cycles) {
  for (uint32_t i = 0; i < cycles; i++) {
    __asm__("nop");
  }
}

const i2c_controller_config_t i2c_default_sm_config = I2C_DEFAULT_SM_CONFIG;

i2c_controller_status_t i2c_controller_init(const i2c_controller_config_t *config) {
  if (!config) {
    return I2C_CONTROLLER_ERROR;
  }

  /* Disable I2C peripheral before making changes */
  i2c_peripheral_disable(config->i2c);

  i2c_set_clock_frequency(config->i2c, config->clock_freq);

  i2c_set_ccr(config->i2c, config->ccr);

  i2c_set_trise(config->i2c, config->trise);

  i2c_peripheral_enable(config->i2c);

  return I2C_CONTROLLER_SUCCESS;
}

i2c_controller_status_t i2c_controller_scan(uint32_t i2c) {
  uint32_t counter = 0;

  for (uint8_t address = 1; address < 128; address++) {
    counter = 96000000 / 256;

    while ((I2C_SR2(i2c) & (I2C_SR2_BUSY))) {}

    i2c_send_start(i2c);

    while (!((I2C_SR1(i2c) & I2C_SR1_SB) && (I2C_SR2(i2c) & (I2C_SR2_MSL | I2C_SR2_BUSY)))) {}

    i2c_send_7bit_address(i2c, address, I2C_WRITE);

    while (!(I2C_SR1(i2c) & I2C_SR1_ADDR)) {
      counter--;
      if (!counter) {
        break;
      }
    }

    (void)I2C_SR2(i2c);

    if (counter) {
      message_write(data_buffer, "Address found: ");
      usart_controller_send(USART1, data_buffer);

      message_dtoh_string(address, hex_string);
      message_write(data_buffer, hex_string);
      usart_controller_send(USART1, data_buffer);
    }

    i2c_send_stop(i2c);
  }
   
  return I2C_CONTROLLER_SUCCESS;
}

i2c_controller_status_t i2c_controller_send_byte(uint32_t i2c, uint8_t address, const uint8_t *data) {
  if (!data) {
    return I2C_CONTROLLER_ERROR;
  }

  while ((I2C_SR2(i2c) & (I2C_SR2_BUSY))) {} 

  message_write(data_buffer, "start\r\n");
  usart_controller_send(USART1, data_buffer);

  /* A serial data transfer always begins with a start condition 
   * and ends with a stop condition */
  i2c_send_start(i2c);

  /* Setting the START bit causes the interface to generate a Start condition and to switch 
   * to Master mode (MSL bit set) when the BUSY bit is cleared. */
  while (!((I2C_SR1(i2c) & I2C_SR1_SB) && (I2C_SR2(i2c) & (I2C_SR2_MSL | I2C_SR2_BUSY)))) {}

  message_write(data_buffer, "address send\r\n");
  usart_controller_send(USART1, data_buffer);
  i2c_send_7bit_address(i2c, address, I2C_WRITE);
  
  /* Waiting for address is transferred. */
	while (!(I2C_SR1(i2c) & I2C_SR1_ADDR)) {}

  message_write(data_buffer, "clean address flag\r\n");
  usart_controller_send(USART1, data_buffer);
  /* Cleaning the address flag */
  I2C_SR2(i2c);

  /* Wait until the TxE buffer is empty. */
  // while (!(I2C_SR1(i2c) & I2C_SR1_TxE)) {
  //   message_write(data_buffer, "wait until the txe buffer is empty\r\n");
  //   usart_controller_send(USART1, data_buffer);
  // }
  i2c_send_data(i2c, *data);

  /* Wait until the byte transfer is finished. */
  while (!(I2C_SR1(i2c) & I2C_SR1_BTF)) {}

  i2c_send_stop(i2c);

  return I2C_CONTROLLER_SUCCESS;
} 
