
#include "tnix/assert.h"
#include "arch/arch.h"
#include "arch/vram.h"
#include "arch/v9990.h"

#define FONT_PAGE            0x06
#define FONT_OFFSET          0x8000
#define FONT_ADDRESS         (((uint32_t) FONT_PAGE << 16) |  FONT_OFFSET)

static unsigned short vram_x, vram_y, vram_height, vram_ncols, vram_nrows;


void vram_size(unsigned char cols, unsigned char rows)
{
     assert(cols <= 128 && rows <= 48);
     vram_ncols = cols;
     vram_nrows = rows;
}


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

     waitce();
     v9990cmd(&LMMV);
}


void prev_vram(void)
{
}


void next_vram(void)
{
     if ((vram_x += 8) > vram_ncols * 8) {
          vram_x = 0;
          vram_y += vram_height;
     }
}




void ptr_vram(uint8_t x, uint8_t y)
{
     assert(x < vram_ncols &&  y < vram_nrows);
     vram_x = x * 8;
     vram_y = y * vram_height;
}



void write_vram(unsigned char c, struct video_att att)
{
     static struct CMMMpars CMMM = {
          0, 0,
          0, FONT_PAGE,
          0, 0,
          8, 0,
          0, V9990_LOGIC_IMP,
          0xffff,
          0x3333,0x5555,
          V9990_OP_CMMM
     };

     uint16_t offset = FONT_OFFSET + c * 32;

     CMMM.sa_off1 = offset & 0xff;
     CMMM.sa_off2 = (offset >> 8) & 0xff;
     CMMM.dx = vram_x;
     CMMM.dy = vram_y;
     CMMM.ny = vram_height;

     CMMM.bg = att.bg;
     CMMM.bg |= CMMM.bg << 4;
     CMMM.bg |= CMMM.bg << 8;

     CMMM.fg = att.fg;
     CMMM.fg |= CMMM.fg << 4;
     CMMM.fg |= CMMM.fg << 8;

     waitce();
     v9990cmd(&CMMM);
}



void hide_cursor(void)
{
     resetspd();
}


void enable_cursor(void)
{
     setspd();
     set_cursor_pos(vram_x, vram_y);
}



/**
 * read all font glyphs to vram from a ram buffer.There is 256 fonts size
 * of 8xheight size
 **/

uint8_t read_glyphs(uint8_t *buf_font)
{
     /* TODO:Implement this function */
     return 0;
}



/**
 * write all font  glyphs to vram from a ram buffer. There is 256 fonts size
 * of 8xheight size
 **/


void write_glyphs(uint8_t *buf_font, uint8_t height)
{
     static uint8_t i;
     static const int8_t zero[16];

     assert(buf_font);
     assert(height >= 8 && height <= 16);
     vram_height = height;
     vwrite_ad(FONT_ADDRESS);

     do {
          ldirvm(buf_font, height);
          if (height != 16)    ldirvm(zero, 16 - height);

          ldirvm(zero, 16);
          buf_font += 16;
     } while (--i);                /* 256 times using only a byte */
}


