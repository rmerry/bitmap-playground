#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define NOT_A_VALID_BMP_FILE "Not a valid bitmap (.bmp) file"
#define BITMAPV5HEADER_SIZE 124
#define BITMAPINFOHEADER_SIZE 40

void fail_with_message(char *);
bool read_header(FILE **);
bool read_dib_header(FILE **);

int main(int argc, char *argv[])
{
  if (argc < 2) {
    fail_with_message("You must specify a bitmap (.bmp) file.");
  }

  FILE *fp = fopen(argv[1], "r+");

  if (!read_header(&fp)) fail_with_message("Invalid header");
  if (!read_dib_header(&fp)) fail_with_message("Invalid dib header");

  fclose(fp);

  return 0;
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
 * The DIB header holds important information about the bitmap image. This
 * header begins at ofset 0x1E and immediately follows the main header.
 */
bool
read_dib_header(FILE **fp)
{
  unsigned char *dib_header_size = calloc(4, sizeof(unsigned char));

  // Read the DIB header size into memory
  if(fread(dib_header_size, sizeof(unsigned char), 4, *fp) != 4) {
    fail_with_message(NOT_A_VALID_BMP_FILE);
  }

  uint32_t size = (uint32_t)dib_header_size[0x03] << 24 |
                  (uint32_t)dib_header_size[0x02] << 16 |
                  (uint32_t)dib_header_size[0x01] << 8  |
                  (uint32_t)dib_header_size[0x00];

  printf("the dib header size is %d bytes\n", size);

  unsigned char *dib_header = calloc(size-4, sizeof(unsigned char));

  // Read the DIB header into memory
  if(fread(dib_header, sizeof(unsigned char), size-4, *fp) != size-4) {
    fail_with_message(NOT_A_VALID_BMP_FILE);
  }

  uint16_t bits_per_pixel;
  uint32_t compression_method;
  uint16_t colour_planes;
  int32_t height;
  int32_t width;

  switch (size) {
    case BITMAPINFOHEADER_SIZE:
      break;

    case BITMAPV5HEADER_SIZE:

      width = (int32_t)dib_header[0x03] << 24 |
              (int32_t)dib_header[0x02] << 16 |
              (int32_t)dib_header[0x01] << 8  |
              (int32_t)dib_header[0x00];

      height = (int32_t)dib_header[0x07] << 24 |
               (int32_t)dib_header[0x06] << 16 |
               (int32_t)dib_header[0x05] << 8  |
               (int32_t)dib_header[0x04];

      // Should be 1
      colour_planes = (uint16_t)dib_header[0x09] << 8  |
                      (uint16_t)dib_header[0x08];

      bits_per_pixel = (uint16_t)dib_header[0x0B] << 8  |
                       (uint16_t)dib_header[0x0A];

      compression_method = (uint32_t)dib_header[0x0F] << 24 |
                           (uint32_t)dib_header[0x0E] << 16 |
                           (uint32_t)dib_header[0x0D] << 8  |
                           (uint32_t)dib_header[0x0C];


      break;
  }

  printf("the image width is %d pixels\n", width);
  printf("the image height is %d pixels\n", height);
  printf("there are %d colour planes\n", colour_planes);
  printf("there are %d bits per pixel\n", bits_per_pixel);
  printf("the compression method is %d\n", compression_method);

  return true;
}

/*
 * Reads a bitmap file header (first 14 bytes)
 */
bool
read_header(FILE **fp)
{
  unsigned char *header = calloc(14, sizeof(unsigned char));

  // Read the header into memory
  if(fread(header, sizeof(unsigned char), 14, *fp) != 14) {
    fail_with_message(NOT_A_VALID_BMP_FILE);
  }

  if (header[0x00] != 0x42) fail_with_message(NOT_A_VALID_BMP_FILE);
  if (header[0x01] != 0x4D) fail_with_message(NOT_A_VALID_BMP_FILE);

  // That sounds like Jesus level programming -- Rob Bollons (10/08/2016)
  uint32_t file_size = (uint32_t)header[0x05] << 24 |
                       (uint32_t)header[0x04] << 16 |
                       (uint32_t)header[0x03] << 8  |
                       (uint32_t)header[0x02];

  printf("the file size is %d bytes\n", file_size);

  // Skip the reseved 4 bytes

  uint32_t pixel_array_offset = (uint32_t)header[0x0A] << 24 |
                                (uint32_t)header[0x0B] << 16 |
                                (uint32_t)header[0x0C] << 8  |
                                (uint32_t)header[0x0D];

  printf("the pixel array offset is %08X\n", pixel_array_offset);

  free(header);

  return true;
}
