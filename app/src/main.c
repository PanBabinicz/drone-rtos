#include "common-defines.h"
#include "i2c_controller.h"
#include "usart_controller.h"
#include <message.h>
#include <FreeRTOS.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/rcc.h>

static void rcc_setup(void) {
  /* Clock configuration */
  rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_96MHZ]);
  
  /* Enable GPIOC */
  rcc_periph_clock_enable(RCC_GPIOC);

  /* Enable GPIOB */
  rcc_periph_clock_enable(RCC_GPIOB);
  
  /* Enable GPIOA */
  rcc_periph_clock_enable(RCC_GPIOA);

  /* Enable I2C1 */
  rcc_periph_clock_enable(RCC_I2C1);

  /* Enable USART1_TX */
  rcc_periph_clock_enable(RCC_USART1);
}

static void gpio_setup(void) {
  /* Configure GPIO port and pins for LED2 */
  gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);

  /* Configure GPIO port and pins for I2C1 */
  gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO6 | GPIO7);

  /* Configure GPIO port and pin for USART1_TX */
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);

  /* Set alternative function for I2C1 */
  gpio_set_af(GPIOB, GPIO_AF4, GPIO6 | GPIO7);

  /* Set alternative function for USART1_TX */
  gpio_set_af(GPIOA, GPIO_AF7, GPIO9);
}

static void i2c_setup(void) {
  i2c_peripheral_disable(I2C1);
  i2c_set_standard_mode(I2C1);
  i2c_peripheral_enable(I2C1);
}

static void usart_setup(void) {
  usart_controller_init(&usart_default_config);
}

static void delay(uint32_t cycles) {
  for (uint32_t i = 0; i < cycles; i++) {
    __asm__("nop");
  }
}

static void led2_on(void) {
  /* PIN is in a sink mode */
  gpio_clear(GPIOC, GPIO13);
}

static void led2_off(void) {
  /* PIN is in a sink mode */
  gpio_set(GPIOC, GPIO13);
}

int main(void) {
  rcc_setup();
  gpio_setup();
  i2c_setup();
  usart_setup();

  led2_off();

  uint16_t data_buffer[128];
  message_write(data_buffer, "Hello from this side!\n\r");

  while (1) {
    delay(96000000 / 4);
    i2c_controller_send(I2C1, &(uint8_t){0b00110101});
    usart_controller_send(USART1, data_buffer);
  }

  return 0;
}
