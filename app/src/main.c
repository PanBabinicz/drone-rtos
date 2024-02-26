#include "common-defines.h"
#include "i2c_controller.h"
#include "usart_controller.h"
#include "mpu6050.h"
#include "tim_controller.h"
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
  
  /* Enable TIM3 clock. */
	rcc_periph_clock_enable(RCC_TIM3);

  /* Reset TIM3 peripheral to defaults. */
	rcc_periph_reset_pulse(RST_TIM3);
}

static void gpio_setup(void) {
  /* Configure GPIO port and pins for LED2 */
  gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);

  /* Configure GPIO port and pins for I2C1 */
  gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO6 | GPIO7);
  // gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO6 | GPIO7);

  /* Configure GPIO port and pin for USART1_TX */
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);

  /* Configure GPIO port and pin for TIM3_CH1 and TIM3_CH2 */
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO6 | GPIO7);

  /* Configure GPIO port and pin for TIM3_CH3 and TIM3_CH4 */
  gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO0 | GPIO1);

  /* Set alternative function for I2C1 */
  gpio_set_af(GPIOB, GPIO_AF4, GPIO6 | GPIO7);

  /* Set alternative function for USART1_TX */
  gpio_set_af(GPIOA, GPIO_AF7, GPIO9);
  
  /* Set alternative function for TIM3_CH1 and TIM3_CH2 */
  gpio_set_af(GPIOA, GPIO_AF2, GPIO6 | GPIO7);

  /* Set alternative function for TIM3_CH1 and TIM3_CH2 */
  gpio_set_af(GPIOB, GPIO_AF2, GPIO0 | GPIO1);
}

static void i2c_setup(void) {
  i2c_controller_init(&i2c_default_sm_config);
}

static void usart_setup(void) {
  usart_controller_init(&usart_default_config);
}

static void tim_setup(void) {
  tim_controller_init(&tim3_default_config);
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
  tim_setup();

  delay(96000000 / 4);
  led2_on();

  delay(96000000 / 4);
  led2_off();

  delay(96000000 / 32);
  mpu6050_reset(I2C1);

  delay(96000000 / 32);
  mpu6050_sleep_off(I2C1);

  delay(96000000 / 32);
  mpu6050_config(I2C1);

  delay(96000000 / 32);
  mpu6050_get_pwr_mgmt_1(I2C1);

  mpu6050_accel_data_t accel = { .x = 0, .y = 0, .z = 0 };
  mpu6050_gyro_data_t gyro = { .x = 0, .y = 0, .z = 0 };
  mpu6050_temp_data_t temp = { .raw_temp = 0, .celsius = 0 };

  mpu6050_set_prescalers(&accel, &gyro);

  char data_buffer[100];

  delay(96000000 / 32);
  int length = snprintf(data_buffer, sizeof(data_buffer), "Address found: 0x%x\r\n", 105);
  usart_controller_send(USART1, data_buffer, (uint16_t)length);

  delay(96000000 / 4);
  led2_off();

  while (1) {
    tim_controller_display(USART1);

    delay(96000000 / 64);
    length = snprintf(data_buffer, sizeof(data_buffer), "\033[2J");
    usart_controller_send(USART1, data_buffer, (uint16_t)length);

    length = snprintf(data_buffer, sizeof(data_buffer), "\033[H");
    usart_controller_send(USART1, data_buffer, (uint16_t)length);
  }

  return 0;
}
