#ifndef _VRAM_H_
#define _VRAM_H_

#include "arch/types.h"


struct video_att {
  /* attribute flags */
  unsigned bold      : 1;
  unsigned underline : 1;
  unsigned reverse   : 1;
  unsigned bg        : 3;
  unsigned fg        : 3;
};

#define CLEAR_VIDEO(x) (*(uint16_t *)&(x) = 0)

void prev_vram(void);
void next_vram(void);
void ptr_vram(uint8_t x, uint8_t y);
void write_vram(unsigned char c, struct video_att att);
void hide_cursor(void);
void enable_cursor(void);
void write_glyphs(uint8_t *buf_font, uint8_t height);
uint8_t read_glyphs(uint8_t *buf_font);
void clean_vram(void);
#endif /* _VRAM_H_ */
