#include <intrpt.h>
#include <sys.h>
#include <assert.h>

#include "vram.h"
#include "v9990.h"


#define FONT_PAGE            6
#define FONTS_ADDRESS        0
#define HEIGHT_ADDRESS       0xffff



void prev_vram(void)
{
}


void next_vram(void)
{
}




void ptr_vram(uint8_t x, uint8_t y, uint8_t nrows)
{
}




void write_vram(int16_t c, struct video_att att)
{
}



void hide_cursor(void)
{
}


void enable_cursor(void)
{
}



void set_height_glyph(uint8_t height)
{

  vwrite_ad(HEIGHT_ADDRESS, FONT_PAGE);
  outp(V9990_VRAM_PORT, height);
  ei();

  assert(get_height_glyph() == height);
}


uint8_t get_height_glyph(void)
{
  register uint8_t ret;
  vread_ad(HEIGHT_ADDRESS, FONT_PAGE);
  ret = inp(V9990_VRAM_PORT);
  ei();

  assert(ret >= 8 && ret <= 32);
  return ret;
}



/**
 * read all font glyphs to vram from a ram buffer.There is 256 fonts size
 * of 8xheight size
 **/

int8_t read_glyphs(uint8_t *buf_font)
{
  uint8_t i, j, height = get_height_glyph();

  assert(buf_font);
  assert(height >= 8 && height <= 32);
  vread_ad(FONTS_ADDRESS, FONT_PAGE), i = 0;
  do {
    for (j = height; j != 0; --j) {
      register uint8_t byte = 0, k;
      for (k = 8; k != 0; --k) {
        byte <<= 1;
        byte |= inp(V9990_VRAM_PORT) & 1;
      }

      *buf_font++ = byte;
    }
  } while (--i);                /* 256 times using only a byte */

  ei();
  return height;
}



/**
 * write all font  glyphs to vram from a ram buffer. There is 256 fonts size
 * of 8xheight size
 **/

void write_glyphs(uint8_t *buf_font, int8_t height)
{
  uint8_t i, j;

  assert(buf_font);
  assert(height >= 8 && height <= 32);
  set_height_glyph(height);
  vwrite_ad(FONTS_ADDRESS, FONT_PAGE), i = 0;
  do {
    for (j = height; j != 0; --j) {
      register uint8_t byte = *buf_font++, k;
      for (k = 8; k != 0; --k) {
        outp(V9990_VRAM_PORT, byte & 1);
        byte >>= 1;
      }
    }
  } while (--i);                /* 256 times using only a byte */

  ei();
}


