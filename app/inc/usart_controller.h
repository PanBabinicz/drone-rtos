#ifndef INC_USART_CONTROLLER_H
#define INC_USART_CONTROLLER_H

#include <common-defines.h>
#include <message.h>

#include <libopencm3/stm32/usart.h>

typedef enum {
  USART_CONTROLLER_SUCCESS = 0,
  USART_CONTROLLER_NULL_POINTER,
  USART_CONTROLLER_ERROR,
} usart_controller_status_t;

typedef struct {
  uint32_t usart;
  uint32_t baudrate;
  uint32_t databits;
  uint32_t flow_control;
  uint32_t mode;
  uint32_t parity;
  uint32_t stopbits;
} usart_controller_config_t;

#define USART_DEFAULT_CONFIG { \
  .usart = USART1, \
  .baudrate = 115200, \
  .databits = 8, \
  .flow_control = USART_FLOWCONTROL_NONE, \
  .mode = USART_MODE_TX, \
  .parity = USART_PARITY_NONE, \
  .stopbits = USART_STOPBITS_1, \
};

extern usart_controller_config_t usart_default_config;

usart_controller_status_t usart_controller_init(const usart_controller_config_t *config);
usart_controller_status_t usart_controller_send(uint32_t usart, uint16_t *data_buffer);

#endif // !INC_USART_CONTROLLER_H
