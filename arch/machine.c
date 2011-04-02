/* -*- mode: c; coding: iso-8859-1 -*- */

#include "arch/arch.h"
#include "arch/v9990.h"
#include "arch/vram.h"
#include "arch/types.h"

#define STACK_ITEMS 300
#define GLYPH_SIZE  16

union stacku {
     char c[2];
     short s;
};

union stacku kernel_stack[STACK_ITEMS];
union stacku *stack_init = &kernel_stack[STACK_ITEMS];




/*
 * THIS IS ONLY TEMPORAL. THIS MUST BE MOVED TO THE LOADER
 */

static uint8_t fonts[] = {
#include "fonts.h"
};


void arch_init(void)
{
     vdp_init();
     write_glyphs(fonts, GLYPH_SIZE);
}
