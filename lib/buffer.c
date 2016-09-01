#include "buffer.h"

unsigned char
get_byte(struct buffer *buf)
{
 if (buf->index >= buf->length) 
   return 0;

 return buf->data[buf->index++];
}
