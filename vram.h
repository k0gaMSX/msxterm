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

void vram_prev(void);
void vram_next(void);
void vram_ptr(uint8_t x, uint8_t y, uint8_t nrows);

void vram_write(int16_t c, struct video_att att);
void hide_cursor(void);
void enable_cursor(void);
#endif /* _VRAM_H_ */
