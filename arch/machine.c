/* -*- mode: c; coding: iso-8859-1 -*- */

#include "arch.h"

#define STACK_ITEMS 300
#define GLYPH_SIZE  16

union stacku {
     char c[2];
     short s;
};

union stacku kernel_stack[STACK_ITEMS];
union stacku *stack_init = &kernel_stack[STACK_ITEMS];


void arch_init(void)
{

}
