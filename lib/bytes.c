#include <stdint.h>

typedef unsigned char byte;

uint16_t
read_uint16_le(byte *buffer)
{
  return (uint16_t)buffer[1] << 8 |
         (uint16_t)buffer[0];
}

uint32_t
read_uint32_le(byte *buffer)
{
  /* That sounds like Jesus level programming -- Rob Bollons (10/08/2016) */
  return (uint32_t)buffer[3] << 24 |
         (uint32_t)buffer[2] << 16 |
         (uint32_t)buffer[1] << 8  |
         (uint32_t)buffer[0];
}
