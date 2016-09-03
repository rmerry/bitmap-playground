#ifndef _BYTES_H_
#define _BYTES_H_

#include "buffer.h"

/* function prototypes */

uint16_t read_uint16_le(struct buffer *);
uint32_t read_uint32_le(struct buffer *);
int32_t read_int32_le(struct buffer *);
unsigned char *read_bit_array_le(struct buffer *);

#endif
