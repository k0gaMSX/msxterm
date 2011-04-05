
OBJS_CRT = asm/imul.obj asm/bitfield.obj asm/wrelop.obj asm/wrelop1.obj \
	   asm/wrelop2.obj asm/wrelop3.obj asm/csv.obj asm/idiv.obj
OBJS_ASM = asm/memset.obj
OBJS_ARCH = machine.obj asm/v9958.obj asm/v9990.obj asm/kbd.obj vram.obj
OBJS = $(OBJS_CRT) $(OBJS_ARCH) $(OBJS_ASM)
DEPS = $(OBJS:.obj=.d)



all: head.obj arch.lib crt0.lib asm.lib

asm.lib: $(OBJS_ASM)

crt0.lib: $(OBJS_CRT)

arch.lib: $(OBJS_ARCH)

clean:
	rm -f *.obj *.lib asm/*.obj


include Makefile.rules
### Dependencies:
