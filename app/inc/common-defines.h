#ifndef INC_COMMON_DEFINES_H
#define INC_COMMON_DEFINES_H
#define PRINTF_DEBUG 1

#include <stdint.h>

#ifdef PRINTF_DEBUG
# include <printf.h>
#endif

#define RCC_CLOCK_SPEED_HZ  (96000000)

#endif // !INC_COMMON_DEFINES_H
