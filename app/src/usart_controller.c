#include <usart_controller.h>

usart_controller_config_t usart_default_config = USART_DEFAULT_CONFIG;

usart_controller_status_t usart_controller_init(const usart_controller_config_t *config) {
  if (!config) {
    return USART_CONTROLLER_ERROR;
  }

  usart_disable(config->usart);
  usart_set_baudrate(config->usart, config->baudrate);
  usart_set_databits(config->usart, config->databits);
  usart_set_flow_control(config->usart, config->flow_control);
  usart_set_mode(config->usart, config->mode);
  usart_set_parity(config->usart, config->parity);
  usart_set_stopbits(config->usart, config->stopbits);
  usart_enable(config->usart);

  return USART_CONTROLLER_SUCCESS;
}

usart_controller_status_t usart_controller_send(uint32_t usart, uint16_t *data_buffer) {
  if (!data_buffer) {
    return USART_CONTROLLER_NULL_POINTER;
  }

  for (uint16_t i = 0; i < message_length(data_buffer); i++) {
    usart_send_blocking(usart, data_buffer[i]);
  }

  return USART_CONTROLLER_SUCCESS;
}

