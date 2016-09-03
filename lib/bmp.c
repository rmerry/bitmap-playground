#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include "bytes.h"
#include "buffer.h"

#define NOT_A_VALID_BMP_FILE "Not a valid bitmap (.bmp) file"

void fail_with_message(char *);
bool load_bitmap(struct buffer *);
bool load_buffer(char *, struct buffer *);

int
main(int argc, char *argv[])
{
  if (argc < 2) {
    fail_with_message("You must specify a bitmap (.bmp) file.");
  }

  struct buffer buf;

  if (!load_buffer(argv[1], &buf))
    fail_with_message("Error creating buffer.");

  printf("The length of the buffer is %u bytes\n", buf.length);

  if (!load_bitmap(&buf)) 
    fail_with_message("Invalid header");

  /* free((void *)buf->data); */

  return 0;
}

/*
 *
 */
bool
load_buffer(char *fileName, struct buffer *buf)
{
  FILE *fp = fopen(fileName, "r+");

  fseek(fp, 0L, SEEK_END);
  int len = ftell(fp);
  rewind(fp);

  buf->index = 0;
  buf->length = len;
  buf->data = malloc(sizeof(unsigned char)*len);

  fread(buf->data, sizeof(unsigned char), len, fp);

  fclose(fp);

  return true;
}

/* 
 * Prints an error message to stdout and
 * returns to OS with code 1
 */
void
fail_with_message(char *msg)
{
  printf("%s\n", msg);
  exit(1);
}

/*
 * Reads a bitmap file header (first 14 bytes)
 */
bool
load_bitmap(struct buffer *buf)
{
  int32_t width                   = 0;
  int32_t height                  = 0;
  int32_t important_colours       = 0;
  int32_t px_per_meter_horizontal = 0;
  int32_t px_per_meter_vertical   = 0;
  uint16_t bpp                    = 0;
  uint16_t colour_planes          = 0;
  uint16_t colours_in_palette     = 0;
  uint32_t bmp_data_size          = 0;
  uint32_t compression_method     = 0;
  uint32_t file_size              = 0;
  uint32_t px_array_offset        = 0;
  uint32_t dib_size               = 0;

  /* reads a bitmap file header (first 14 bytes) */

  /* check magic numbers */
  if (get_byte(buf) != 0x42) fail_with_message(NOT_A_VALID_BMP_FILE);
  if (get_byte(buf) != 0x4D) fail_with_message(NOT_A_VALID_BMP_FILE);

  file_size = read_uint32_le(buf);

  /* skip the reseved 2x2 bytes */
  buf->index += 4;

  px_array_offset = read_uint32_le(buf);

 /* The DIB header holds important information about the bitmap image. This
  * header begins at ofset 0x0E and immediately follows the main header. */

  /* Read the DIB header size into memory */
  dib_size = read_uint32_le(buf);

  /* We utilise the size of the DIB header to determine the bitmap format */
  switch (dib_size) {
    case BITMAPCOREHEADER_SIZE:
      width          = (int32_t)read_uint16_le(buf);
      height         = (int32_t)read_uint16_le(buf);
      colour_planes  = read_uint16_le(buf);
      bpp = read_uint16_le(buf);
      break;

    case BITMAPINFOHEADER_SIZE:
    case BITMAPV5HEADER_SIZE:
      width                    = read_int32_le(buf);
      height                   = read_int32_le(buf);
      colour_planes            = read_uint16_le(buf);
      bpp           = read_uint16_le(buf);
      compression_method       = read_uint32_le(buf);
      bmp_data_size            = read_uint32_le(buf);
      pix_per_meter_horizontal = read_int32_le(buf);
      pix_per_meter_vertical   = read_int32_le(buf);
      colours_in_palette       = read_int32_le(buf);
      important_colours        = read_uint32_le(buf);
      break;
  }

 /* read the colour palette: all bitmap files <= 8bits must have a 
  * colour palette; bitmap files with a higher bit density can 
  * optionally contain a colour palette. */

  if (buf->index < px_array_offset) {
    int palette_size = (colours_in_palette == 0) ? 2^bpp : colours_in_palette;

    if (bpp == 1) {
      struct rgbaquad palette[palette_size];

      int i;
      for (i = 0; i < palette_size; ++i) {
        palette[i].red   = get_byte(buf);
        palette[i].green = get_byte(buf);
        palette[i].blue  = get_byte(buf);
        palette[i].alpha = get_byte(buf);
      }
    }

  /* read the bitmap array */

  /* This should never be the case, best attempts if it does */
  if (buf->index != px_array_offset)
    buf->index = px_array_offset;

  switch (bpp) {
    case 1:
      struct rgbaquad pixel_array[width*height];
      int i;
      for (i = 0; i < palette_size; ++i) {
        palette[i*width+l].red   = get_byte(buf);
        palette[i*width+l].green = get_byte(buf);
        palette[i*width+l].blue  = get_byte(buf);
        palette[i*width+l].alpha = get_byte(buf);
      }

      break;
  }

  /* display information about the file */
  printf("the file size is %d bytes\n", file_size);
  printf("the pixel array offset is %d\n", pixel_array_offset);
  printf("the dib header size is %d bytes\n", dib_size);
  printf("the image width is %d pixels\n", width);
  printf("the image height is %d pixels\n", height);
  printf("there are %d colour planes\n", colour_planes);
  printf("there are %d bits per pixel\n", bits_per_pixel);
  printf("the compression method is %d\n", compression_method);
  printf("bitmap data size: %d\n", bmp_data_size);
  printf("pixels per meter (horizontal): %d\n", pix_per_meter_horizontal);
  printf("pixels per meter (vertical): %d\n", pix_per_meter_vertical);
  printf("colours in palette:  %d\n", colours_in_palette);
  printf("important colours:  %d\n", important_colours);
  printf("The index of the buffer [post palette]: %d\n", buf->index);

  return true;
}
