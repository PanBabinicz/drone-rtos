#ifndef INC_MESSAGE_H
#define INC_MESSAGE_H

#include <common-defines.h>

void message_dtoh_string(uint16_t decimal, char *hex_string);
void message_write(uint16_t *data_buffer, char *message);
uint16_t message_length(uint16_t *message); 

#endif // !INC_MESSAGE_H
