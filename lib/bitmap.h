/*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

#include <stdlib.h>

#define BITMAPCOREHEADER_SIZE 12
#define BITMAPINFOHEADER_SIZE 40
#define BITMAPV5HEADER_SIZE 124

/*
 * Platforms: Windows 2.0+ / OS/2 1.x
 * Notes: These bitmaps are uncompressed and do not support
 *        bits per pixel values of 16 or 32
 */
struct bitmapcoreheader
{
  uint16_t width;
  uint16_t height;
  uint16_t colour_planes; /* Must be 1 */
  uint16_t bits_per_pixel; /* Color depth; i.e., 1,2,4,8,16,24,32 */
} BITMAPCOREHEADER;

/*
 *
 *
 */
struct bitmapinfoheader
{
  int32_t width;
  int32_t height;
  uint16_t colour_planes; /* Must be 1 */
  uint16_t bits_per_pixel; /* Color depth; i.e., 1,2,4,8,16,24,32 */
  uint32_t compression_method; /* This can be 0 for BI_RGB bitmap types */
  uint32_t raw_pixel_data_size_bytes;
  int32_t horizonal_resolution; /* Pixels per meter */
  int32_t vertical_resolution; /* Pixels per meter */
  int32_t colours_in_palette; /* 0 to default to 2^n */
  int32_t important_colours; /* 0 to indicate all colours are important */
} BITMAPINFOHEADER;

/* The RGBA Quad data structure is used to store an
 * RGAA 4-tuple as used in the colour palette data
 * and the [FILL ME IN]
 */
struct rgbaquad
{
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  unsigned char alpha;
} RGBAQUAD;
