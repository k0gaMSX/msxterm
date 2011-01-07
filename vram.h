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

#define CLEAR_VIDEO(x) (*(u16 *)&(x) = 0)
#define CLEAR_VIDEO_S(x) (*(u16 *)&(x) = 0)

void vram_prev(void);
void vram_next(void);
void vram_ptr(u8 x, u8 y, u8 nrows);

void vram_write(u16 c, struct video_att att);
void hide_cursor(void);
void enable_cursor(void);
#endif /* _VRAM_H_ */
