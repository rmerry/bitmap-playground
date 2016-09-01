#include <stdint.h>
#include "buffer.h"

typedef unsigned char byte;

uint16_t
read_uint16_le(struct buffer *buf)
{
  byte bytes[2];

  bytes[0] = get_byte(buf);
  bytes[1] = get_byte(buf);

  return (uint16_t)bytes[1] << 8 |
                   bytes[0];
}

uint32_t
read_uint32_le(struct buffer *buf)
{
  byte bytes[4];

  bytes[0] = get_byte(buf);
  bytes[1] = get_byte(buf);
  bytes[2] = get_byte(buf);
  bytes[3] = get_byte(buf);

  /* That sounds like Jesus level programming -- Rob Bollons (10/08/2016) */
  return (uint32_t)bytes[3] << 24 |
                   bytes[2] << 16 |
                   bytes[1] << 8  |
                   bytes[0];
}

int32_t
read_int32_le(struct buffer *buf)
{
  byte bytes[4];

  bytes[0] = get_byte(buf);
  bytes[1] = get_byte(buf);
  bytes[2] = get_byte(buf);
  bytes[3] = get_byte(buf);

  return (int32_t)bytes[3] << 24 |
                  bytes[2] << 16 |
                  bytes[1] << 8  |
                  bytes[0];
}
