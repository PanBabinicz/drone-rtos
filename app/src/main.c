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

  /* Enable I2C1 */
  rcc_periph_clock_enable(RCC_I2C1);
}

static void gpio_setup(void) {
  /* Configure GPIO port and pins for LED2 */
  gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);

  /* Configure GPIO port and pins for I2C1 */
  gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO6 | GPIO7);

  /* Set alternative function for I2C1 */
  gpio_set_af(GPIOB, GPIO_AF4, GPIO6 | GPIO7);
}

static void i2c_setup(void) {
  i2c_peripheral_disable(I2C1);
  i2c_set_standard_mode(I2C1);
  i2c_peripheral_enable(I2C1);
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

  led2_on();

  while (1) {

  }

  return 0;
}
