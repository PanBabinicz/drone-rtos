#include <message.h>

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
