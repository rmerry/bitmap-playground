#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define NOT_A_VALID_BMP_FILE "Not a valid bitmap (.bmp) file"
#define BITMAPV5HEADER_SIZE 124
#define BITMAPINFOHEADER_SIZE 40

typedef unsigned char byte;

void fail_with_message(char *);
unsigned int loadFile(char *, byte **);
bool readDIBHeader(byte **);
bool readHeader(byte **);

int
main(int argc, char *argv[])
{
  if (argc < 2) {
    fail_with_message("You must specify a bitmap (.bmp) file.");
  }

  byte *buffer = 0;
  unsigned int len;
  len = loadFile(argv[1], &buffer);

  printf("The length of the file is %u bytes\n", len);

  if (!readHeader(&buffer)) fail_with_message("Invalid header");
  /* if (!read_*buffer(&fp)) fail_with_message("Invalid dib header"); */

  free(buffer);

  return 0;
}

/*
 *
 */
unsigned int
loadFile(char *fileName, byte **buffer)
{
  FILE *fp = fopen(fileName, "r+");

  fseek(fp, 0L, SEEK_END);
  int len = ftell(fp);
  rewind(fp);

  *buffer = malloc(sizeof(byte)*len);
  fread(*buffer, sizeof(byte), len, fp);
  fclose(fp);

  return len;
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
readDIBHeader(byte **buffer)
{
  /* unsigned char **header_size = calloc(4, sizeof(unsigned char)); */

  // Read the DIB header size into memory
  // FIXME: these offsets are now wrong
  uint32_t size = (uint32_t)*buffer[0x03] << 24 |
                  (uint32_t)*buffer[0x02] << 16 |
                  (uint32_t)*buffer[0x01] << 8  |
                  (uint32_t)*buffer[0x00];

  printf("the dib header size is %d bytes\n", size);

  uint16_t bits_per_pixel;
  uint32_t compression_method;
  uint16_t colour_planes;
  int32_t height;
  int32_t width;

  switch (size) {
    case BITMAPINFOHEADER_SIZE:
      break;

    case BITMAPV5HEADER_SIZE:

      // FIXME: these offsets are now wrong
      width = (int32_t)*buffer[0x03] << 24 |
              (int32_t)*buffer[0x02] << 16 |
              (int32_t)*buffer[0x01] << 8  |
              (int32_t)*buffer[0x00];

      // FIXME: these offsets are now wrong
      height = (int32_t)*buffer[0x07] << 24 |
               (int32_t)*buffer[0x06] << 16 |
               (int32_t)*buffer[0x05] << 8  |
               (int32_t)*buffer[0x04];

      // Should be 1
      // FIXME: these offsets are now wrong
      colour_planes = (uint16_t)*buffer[0x09] << 8  |
                      (uint16_t)*buffer[0x08];

      // FIXME: these offsets are now wrong
      bits_per_pixel = (uint16_t)*buffer[0x0B] << 8  |
                       (uint16_t)*buffer[0x0A];

      // FIXME: these offsets are now wrong
      compression_method = (uint32_t)*buffer[0x0F] << 24 |
                           (uint32_t)*buffer[0x0E] << 16 |
                           (uint32_t)*buffer[0x0D] << 8  |
                           (uint32_t)*buffer[0x0C];

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
readHeader(byte **buffer)
{
  if (*buffer[0x00] != 0x42) fail_with_message(NOT_A_VALID_BMP_FILE);
  if (*buffer[0x01] != 0x4D) fail_with_message(NOT_A_VALID_BMP_FILE);

  // That sounds like Jesus level programming -- Rob Bollons (10/08/2016)
  uint32_t file_size = (uint32_t)*buffer[0x05] << 24 |
                       (uint32_t)*buffer[0x04] << 16 |
                       (uint32_t)*buffer[0x03] << 8  |
                       (uint32_t)*buffer[0x02];

  printf("the file size is %d bytes\n", file_size);

  // Skip the reseved 4 bytes
  uint32_t pixel_array_offset = (uint32_t)*buffer[0x0A] << 24 |
                                (uint32_t)*buffer[0x0B] << 16 |
                                (uint32_t)*buffer[0x0C] << 8  |
                                (uint32_t)*buffer[0x0D];

  printf("the pixel array offset is %08X\n", pixel_array_offset);

  return true;
}
