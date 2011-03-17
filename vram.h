#ifndef _VRAM_H_
#define _VRAM_H_

#include "types.h"


struct video_att {
  /* attribute flags */
  unsigned bold      : 1;
  unsigned underline : 1;
  unsigned reverse   : 1;
  unsigned bg        : 3;
  unsigned fg        : 3;
};

#define CLEAR_VIDEO(x) (*(uint16_t *)&(x) = 0)
#define CLEAR_VIDEO_S(x) (*(uint16_t *)&(x) = 0)

void prev_vram(void);
void next_vram(void);
void ptr_vram(uint8_t x, uint8_t y);
void write_vram(uint16_t c, struct video_att att);
void hide_cursor(void);
void enable_cursor(void);
uint8_t get_height_glyph(void);
void set_height_glyph(uint8_t height);
void write_glyphs(uint8_t *buf_font, uint8_t height);
int8_t read_glyphs(uint8_t *buf_font);

#endif /* _VRAM_H_ */
