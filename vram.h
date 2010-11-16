#ifndef _VRAM_H_
#define _VRAM_H_

#include "types.h"

void vram_prev(void);
void vram_next(void);
void vram_ptr(u8 x, u8 y, u8 nrows);

void vram_write(u16 c, u8 att);
void hide_cursor(void);
void enable_cursor(void);
#endif /* _VRAM_H_ */
