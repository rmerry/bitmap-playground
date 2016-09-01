#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include "bytes.c"
#include "buffer.c"

#define NOT_A_VALID_BMP_FILE "Not a valid bitmap (.bmp) file"

void fail_with_message(char *);
bool load_buffer(char *, struct buffer *);
bool load_bitmap(struct buffer *);

int
main(int argc, char *argv[])
{
  if (argc < 2) {
    fail_with_message("You must specify a bitmap (.bmp) file.");
  }

  struct buffer buf;

  if (!load_buffer(argv[1], &buf)) {
    fail_with_message("Error creating buffer.");
  }

  printf("The length of the file is %u bytes\n", buf.length);

  if (!load_bitmap(&buf)) fail_with_message("Invalid header");

  free(&buf);

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

  buf->data = malloc(sizeof(byte)*len);
  buf->length = len;
  buf->index = 0;
  fread(buf->data, sizeof(byte), len, fp);
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
  int32_t height;
  int32_t important_colours;
  int32_t pix_per_meter_horizontal;
  int32_t pix_per_meter_vertical;
  int32_t width;
  uint16_t bits_per_pixel;
  uint16_t colour_planes;
  uint16_t colours_in_palette;
  uint32_t bmp_data_size;
  uint32_t compression_method;
  uint32_t file_size;
  uint32_t pixel_array_offset;
  uint32_t size;

  /*
   * Reads a bitmap file header (first 14 bytes)
   */

  if (get_byte(buf) != 0x42) fail_with_message(NOT_A_VALID_BMP_FILE);
  if (get_byte(buf) != 0x4D) fail_with_message(NOT_A_VALID_BMP_FILE);

  file_size =  read_uint32_le(buf);
  /* Skip the reseved 4 bytes */
  pixel_array_offset = read_uint32_le(buf);

  /* /* */
  /*  * The DIB header holds important information about the bitmap image. This */
  /*  * header begins at ofset 0x0E and immediately follows the main header. */
  /*  *1/ */

  /* /1* Read the DIB header size into memory *1/ */
  /* size = read_uint32_le(&buffer[0x0E]); */

  /* /1* We utilise the size of the DIB header to determine the bitmap format *1/ */
  /* switch (size) { */
  /*   case BITMAPCOREHEADER_SIZE: */
  /*     width = (int32_t)read_uint16_le(&buffer[0x12]); */
  /*     height = (int32_t)read_uint16_le(&buffer[0x14]); */
  /*     colour_planes = read_uint16_le(&buffer[0x16]); */
  /*     bits_per_pixel = read_uint16_le(&buffer[0x18]); */
  /*     break; */

  /*   case BITMAPINFOHEADER_SIZE: */
  /*     width = read_int32_le(&buffer[0x12]); */
  /*     height = read_int32_le(&buffer[0x16]); */
  /*     colour_planes = read_uint16_le(&buffer[0x1A]); */
  /*     bits_per_pixel = read_uint16_le(&buffer[0x1C]); */
  /*     compression_method = read_uint32_le(&buffer[0x1E]); */
  /*     bmp_data_size = read_uint32_le(&buffer[0x22]); */
  /*     pix_per_meter_horizontal = read_int32_le(&buffer[0x26]); */
  /*     pix_per_meter_vertical = read_int32_le(&buffer[0x2A]); */
  /*     colours_in_palette = read_int32_le(&buffer[0x2E]); */
  /*     important_colours = read_uint32_le(&buffer[0x32]); */
  /*     break; */

  /*   case BITMAPV5HEADER_SIZE: */
  /*     // FIXME: these offsets are now wrong */
  /*     width = (int32_t)buffer[0x03] << 24 | */
  /*       (int32_t)buffer[0x02] << 16 | */
  /*       (int32_t)buffer[0x01] << 8  | */
  /*       (int32_t)buffer[0x00]; */

  /*     // FIXME: these offsets are now wrong */
  /*     height = (int32_t)buffer[0x07] << 24 | */
  /*       (int32_t)buffer[0x06] << 16 | */
  /*       (int32_t)buffer[0x05] << 8  | */
  /*       (int32_t)buffer[0x04]; */

  /*     // Should be 1 */
  /*     // FIXME: these offsets are now wrong */
  /*     colour_planes = (uint16_t)buffer[0x09] << 8  | */
  /*       (uint16_t)buffer[0x08]; */

  /*     // FIXME: these offsets are now wrong bits_per_pixel = (uint16_t)buffer[0x0B] << 8  | */

  /*     // FIXME: these offsets are now wrong */
  /*     compression_method = (uint32_t)buffer[0x0F] << 24 | */
  /*       (uint32_t)buffer[0x0E] << 16 | */
  /*       (uint32_t)buffer[0x0D] << 8  | */
  /*       (uint32_t)buffer[0x0C]; */

  /*     break; */
  /* } */

  printf("the file size is %d bytes\n", file_size);
  printf("the pixel array offset is %08X\n", pixel_array_offset);
  printf("the dib header size is %d bytes\n", size);
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

  return true;
}
