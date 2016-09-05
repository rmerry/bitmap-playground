#include <stdint.h>
#include "buffer.h"

uint16_t
read_uint16_le(struct buffer *buf)
{
  unsigned char bytes[2];

  int i;
  for(i = 0; i < sizeof(bytes); ++i)
    bytes[i] = get_byte(buf);

  return (uint16_t)bytes[1] << 8 |
                   bytes[0];
}

uint32_t
read_uint32_le(struct buffer *buf)
{
  unsigned char bytes[4];

  int i;
  for(i = 0; i < sizeof(bytes); ++i)
    bytes[i] = get_byte(buf);

  /* That sounds like Jesus level programming -- Rob Bollons (10/08/2016) */
  return (uint32_t)bytes[3] << 24 |
                   bytes[2] << 16 |
                   bytes[1] << 8  |
                   bytes[0];
}

int32_t
read_int32_le(struct buffer *buf)
{
  unsigned char bytes[4];

  int i;
  for(i = 0; i < sizeof(bytes); ++i)
    bytes[i] = get_byte(buf);

  return (int32_t)bytes[3] << 24 |
                  bytes[2] << 16 |
                  bytes[1] << 8  |
                  bytes[0];
}
