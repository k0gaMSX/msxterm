/* -*- mode: c; coding: iso-8859-1 -*- */

#include "arch/arch.h"
#include "arch/v9990.h"
#include "arch/vram.h"
#include "arch/types.h"
#include "arch/stddef.h"
#include "tnix/sys.h"
#include "tnix/assert.h"

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


extern void v9958_init(void);

void arch_init(void)
{
     v9958_init();
     v9990_init();
     write_glyphs(fonts, GLYPH_SIZE);
     request_irq(v9990_handler, NULL);
}

