#include <tim_controller.h>

tim_controller_input_capture_config_t tim3_input_capture_default_config = TIM3_DEFAULT_CONFIG;
tim_controller_output_compare_config_t tim5_output_compare_default_config = TIM5_DEFAULT_CONFIG;

volatile tim_controller_channels_t tim3_channels;
tim_controller_channels_t tim5_channels;

tim_controller_status_t tim_controller_init_input_capture(const tim_controller_input_capture_config_t *config) {
  if (!config) {
    return TIM_CONTROLLER_ERROR;
  }
  
  /* */
  timer_set_mode(config->tim, config->clock_div, config->alignment, config->direction);

  /* */
  timer_set_prescaler(config->tim, config->prescaler);

  /* */
  timer_set_period(config->tim, config->period);

  /* */
  timer_ic_set_input(config->tim, config->input_capture_channel_designator[0], config->input_capture_channel_direction[0]);
  timer_ic_set_input(config->tim, config->input_capture_channel_designator[1], config->input_capture_channel_direction[1]);
  timer_ic_set_input(config->tim, config->input_capture_channel_designator[2], config->input_capture_channel_direction[2]);
  timer_ic_set_input(config->tim, config->input_capture_channel_designator[3], config->input_capture_channel_direction[3]);

  /* */
  timer_ic_set_polarity(config->tim, config->input_capture_channel_designator[0], config->input_capture_channel_polarity[0]);
  timer_ic_set_polarity(config->tim, config->input_capture_channel_designator[1], config->input_capture_channel_polarity[1]);
  timer_ic_set_polarity(config->tim, config->input_capture_channel_designator[2], config->input_capture_channel_polarity[2]);
  timer_ic_set_polarity(config->tim, config->input_capture_channel_designator[3], config->input_capture_channel_polarity[3]);

  /* */
  timer_ic_enable(config->tim, config->input_capture_channel_designator[0]);
  timer_ic_enable(config->tim, config->input_capture_channel_designator[1]);
  timer_ic_enable(config->tim, config->input_capture_channel_designator[2]);
  timer_ic_enable(config->tim, config->input_capture_channel_designator[3]);

  /* */
  timer_enable_counter(config->tim);

  /* */
  timer_enable_irq(config->tim, config->irq);

  /* Enable TIMx interrupt. */
	nvic_enable_irq(config->irqn);

  return TIM_CONTROLLER_SUCCESS;
}

tim_controller_status_t tim_controller_init_output_compare(const tim_controller_output_compare_config_t *config) {
  if (!config) {
    return TIM_CONTROLLER_ERROR;
  }

  /* */
  timer_set_mode(config->tim, config->clock_div, config->alignment, config->direction);

  /* */
  timer_set_prescaler(config->tim, config->prescaler);

  /* */
  timer_set_period(config->tim, config->period);

  /* */
  timer_set_oc_mode(config->tim, config->output_compare_id[0], config->output_compare_mode[0]);
  timer_set_oc_mode(config->tim, config->output_compare_id[1], config->output_compare_mode[1]);
  timer_set_oc_mode(config->tim, config->output_compare_id[2], config->output_compare_mode[2]);
  timer_set_oc_mode(config->tim, config->output_compare_id[3], config->output_compare_mode[3]);

  /* */
  timer_set_oc_fast_mode(config->tim, config->output_compare_id[0]);
  timer_set_oc_fast_mode(config->tim, config->output_compare_id[1]);
  timer_set_oc_fast_mode(config->tim, config->output_compare_id[2]);
  timer_set_oc_fast_mode(config->tim, config->output_compare_id[3]);

  /* */
  timer_enable_oc_output(config->tim, config->output_compare_id[0]);
  timer_enable_oc_output(config->tim, config->output_compare_id[1]);
  timer_enable_oc_output(config->tim, config->output_compare_id[2]);
  timer_enable_oc_output(config->tim, config->output_compare_id[3]);

  /* */
  timer_enable_counter(config->tim);

  return TIM_CONTROLLER_SUCCESS;
}

void tim_controller_check_pwm(void) {
  __asm__("cpsid i");

  if (tim3_channels.current_channel_1 < 900 || tim3_channels.current_channel_1 > 2100) {
    tim3_channels.current_channel_1 = tim3_channels.last_channel_1;
    tim5_channels.current_channel_1 = tim3_channels.current_channel_1;
  } else {
    tim3_channels.last_channel_1 = tim3_channels.current_channel_1;
  }

  if (tim3_channels.current_channel_2 < 900 || tim3_channels.current_channel_2 > 2100) {
    tim3_channels.current_channel_2 = tim3_channels.last_channel_2;
    tim5_channels.current_channel_2 = tim3_channels.current_channel_2;
  } else {
    tim3_channels.last_channel_2 = tim3_channels.current_channel_2;
  }

  if (tim3_channels.current_channel_3 < 900 || tim3_channels.current_channel_3 > 2100) {
    tim3_channels.current_channel_3 = tim3_channels.last_channel_3;
    tim5_channels.current_channel_3 = tim3_channels.current_channel_3;
  } else {
    tim3_channels.last_channel_3 = tim3_channels.current_channel_3;
  }

  if (tim3_channels.current_channel_4 < 900 || tim3_channels.current_channel_4 > 2100) {
    tim3_channels.current_channel_4 = tim3_channels.last_channel_4;
    tim5_channels.current_channel_4 = tim3_channels.current_channel_4;
  } else {
    tim3_channels.last_channel_4 = tim3_channels.current_channel_4;
  }

  __asm__("cpsie i");
}

void tim_controller_display(uint32_t usart) {
  int length;
  char data_buffer[100];

  tim_controller_check_pwm();

  length = snprintf(data_buffer, sizeof(data_buffer), "ch1: %d\n\rch2: %d\n\rch3: %d\n\rch4: %d\n\r", tim3_channels.current_channel_1, 
                    tim3_channels.current_channel_2, tim3_channels.current_channel_3, tim3_channels.current_channel_4);

  usart_controller_send(usart, data_buffer, (uint16_t)length);
}

void tim3_isr(void) {
  /* */
  if (timer_get_flag(TIM3, TIM_SR_CC1IF)) {
    if (GPIO_IDR(GPIOA) & GPIO6) {
      tim3_channels.current_channel_1 = timer_get_counter(TIM3);
    } else if (!(GPIO_IDR(GPIOA) & GPIO6)) {
      tim3_channels.current_channel_1 = timer_get_counter(TIM3) - tim3_channels.current_channel_1;
      timer_set_counter(TIM3, 0x00000000);
    }
    /* Clear compare interrupt flag. */
    timer_clear_flag(TIM3, TIM_SR_CC1IF);

  } else if (timer_get_flag(TIM3, TIM_SR_CC2IF)) {
    if (GPIO_IDR(GPIOA) & GPIO7) {
      tim3_channels.current_channel_2 = timer_get_counter(TIM3);
    } else if (!(GPIO_IDR(GPIOA) & GPIO7)) {
      tim3_channels.current_channel_2 = timer_get_counter(TIM3) - tim3_channels.current_channel_2;
      timer_set_counter(TIM3, 0x00000000);
    }
    /* Clear compare interrupt flag. */
    timer_clear_flag(TIM3, TIM_SR_CC2IF);

  } else if (timer_get_flag(TIM3, TIM_SR_CC3IF)) {
    if (GPIO_IDR(GPIOB) & GPIO0) {
      tim3_channels.current_channel_3 = timer_get_counter(TIM3);
    } else if (!(GPIO_IDR(GPIOB) & GPIO0)) {
      tim3_channels.current_channel_3 = timer_get_counter(TIM3) - tim3_channels.current_channel_3;
      timer_set_counter(TIM3, 0x00000000);
    }
    /* Clear compare interrupt flag. */
    timer_clear_flag(TIM3, TIM_SR_CC3IF);

  } else if (timer_get_flag(TIM3, TIM_SR_CC4IF)) {
    if (GPIO_IDR(GPIOB) & GPIO1) {
      tim3_channels.current_channel_4 = timer_get_counter(TIM3);
    } else if (!(GPIO_IDR(GPIOB) & GPIO1)) {
      tim3_channels.current_channel_4 = timer_get_counter(TIM3) - tim3_channels.current_channel_4;
      timer_set_counter(TIM3, 0x00000000);
    }
    /* Clear compare interrupt flag. */
    timer_clear_flag(TIM3, TIM_SR_CC4IF);
   
  } 
}
