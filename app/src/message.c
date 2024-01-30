#include <message.h>

void message_dtoh_string(uint16_t decimal, char *hex_string) {
  int8_t i = 4;
  uint8_t mask = 0x0F;

  hex_string[i] = '\0';
  i--;

  for (; i >= 0; i--) {
    if ((decimal & mask) < 10) {
      hex_string[i] = '0' + (decimal & mask);
    } else {
      hex_string[i] = 'A' + ((decimal & mask) % 10);
    }

    /* Example:
        * mask = 0xF000;  
        * mask >>= 0x4
        * mask = 0x0F00;
        * ...
        * */
    decimal >>= 0x4;
  }
}

void message_write(uint16_t *data_buffer, char *message) {
  if ((!data_buffer) || (!message)) {
    return;
  }

  while (*message) {
    *data_buffer++ = *message++;
  }

  /* Null terminate sign */
  *data_buffer = 0;
}

uint16_t message_length(uint16_t *message) {
  uint16_t length = 0;

  if (!message) {
    return length;
  }

  while (*message++) {
    length++;
  }

  /* Null terminate sign */
  length++;

  return length;
}
