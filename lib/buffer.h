/*
 *
 *
 *
 */

#include <stdint.h>

struct buffer
{
  unsigned char *data;
  uint32_t index;
  uint32_t length;
};

/* function prototypes */

unsigned char get_byte(struct buffer *);
