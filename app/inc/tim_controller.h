#ifndef INC_TIM_CONTROLLER_H
#define INC_TIM_CONTROLLER_H

#include <common-defines.h>
#include <usart_controller.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>

typedef enum {
  TIM_CONTROLLER_SUCCESS = 0,
  TIM_CONTROLLER_ERROR,
} tim_controller_status_t;

typedef struct {
  uint32_t tim;
  uint32_t clock_div;
  uint32_t alignment;
  uint32_t direction;
  uint32_t prescaler;
  uint32_t period;
  uint32_t irq;
  uint8_t irqn;
  enum tim_ic_id input_capture_channel_designator[4];
  enum tim_ic_input input_capture_channel_direction[4];
  enum tim_ic_pol input_capture_channel_polarity[4];
} tim_controller_config_t;

typedef struct {
  uint16_t current_channel_1;
  uint16_t current_channel_2;
  uint16_t current_channel_3;
  uint16_t current_channel_4;
  uint16_t last_channel_1;
  uint16_t last_channel_2;
  uint16_t last_channel_3;
  uint16_t last_channel_4;
} tim_controller_channels_t;

/* TIM3 is responsible for getting data from the radio receiver. 
 * TIM3->CH1
 * TIM3->CH2
 * TIM3->CH3
 * TIM3->CH4
 **/
#define TIM3_DEFAULT_CONFIG { \
  .tim = TIM3, \
  .clock_div = TIM_CR1_CKD_CK_INT, \
  .alignment = TIM_CR1_CMS_EDGE, \
  .direction = TIM_CR1_DIR_UP, \
  .prescaler = 0x005F, \
  .period = 0xFFFF, \
  .irq = (TIM_DIER_CC1IE | TIM_DIER_CC2IE | TIM_DIER_CC3IE | TIM_DIER_CC4IE), \
  .irqn = (NVIC_TIM3_IRQ), \
  .input_capture_channel_designator = { TIM_IC1, TIM_IC2, TIM_IC3, TIM_IC4 }, \
  .input_capture_channel_direction = { TIM_IC_IN_TI1, TIM_IC_IN_TI2, TIM_IC_IN_TI3, TIM_IC_IN_TI4 }, \
  .input_capture_channel_polarity = { TIM_IC_BOTH, TIM_IC_BOTH, TIM_IC_BOTH, TIM_IC_BOTH}, \
};

/* TIM5 is responsible for setting the ESCs. 
 * TIM5->CH1
 * TIM5->CH2
 * TIM5->CH3
 * TIM5->CH4
 **/

extern tim_controller_config_t tim3_default_config;
extern tim_controller_channels_t tim3_channels;
extern tim_controller_channels_t tim5_channels;

tim_controller_status_t tim_controller_init(const tim_controller_config_t *config);

void tim_controller_display(uint32_t usart);

void tim3_isr(void);

#endif // !INC_TIM_CONTROLLER_H
