#include <intrpt.h>
#include <sys.h>
#include <assert.h>

#include "vram.h"
#include "v9990.h"

#define FONT_PAGE            0x06
#define FONT_OFFSET          0x8000
#define FONT_ADDRESS         (((uint32_t) FONT_PAGE << 16) |  FONT_OFFSET)
#define HEIGHT_ADDRESS       0x6ffff


static uint16_t vram_x, vram_y, vram_height;


void clean_vram(void)
{
  struct LMMVpars LMMV = {
    0,
    0, 0,
    1024, 212,
    0, V9990_LOGIC_IMP,
    0xffff,
    0, 0,
    V9990_OP_LMMV
  };

  di();
  waitce();
  v9990cmd(&LMMV);
  ei();
}


void prev_vram(void)
{
}


void next_vram(void)
{
}




void ptr_vram(uint8_t x, uint8_t y)
{
  assert(x < 1024/8 &&  y < 256/8);
  di();
  vram_x = x * 8;
  vram_height = get_height_glyph();
  vram_y = y * vram_height;
  ei();
}



void write_vram(uint16_t c, struct video_att att)
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
  assert(height >= 8 && height <= 16);

  vwrite_ad(HEIGHT_ADDRESS);
  outp(V9990_VRAM_PORT, height);
  ei();

  assert(get_height_glyph() == height);
}


uint8_t get_height_glyph(void)
{
  register uint8_t ret;
  vread_ad(HEIGHT_ADDRESS);
  ret = inp(V9990_VRAM_PORT);
  ei();

  assert(ret >= 8 && ret <= 16);
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
  assert(height >= 8 && height <= 16);
  vread_ad(FONT_ADDRESS);
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


void write_glyphs(uint8_t *buf_font, uint8_t height)
{
  uint8_t i, j;

  assert(buf_font);
  assert(height >= 8 && height <= 16);
  set_height_glyph(height);
  vwrite_ad(FONT_ADDRESS);

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


