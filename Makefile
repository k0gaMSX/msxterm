
DEP 	= gcc
CC 	= zc
AS 	= zas
CPP     = cpp
CPPFLAGS = -Iinclude -Iinclude/arch
CFLAGS 	= -Zg3 -W-2 -P8 -BS $(CPPFLAGS)
ASFLAGS =
MAKEFLAGS = --include-dir=$(PWD)
AR	= libr
OBJ2HEX = objtohex
LD	= hlink


LIBS 	=  init/init.lib kernel/kernel.lib drivers/char/char.lib arch/arch.lib  \
	   lib/lib.lib arch/asm.lib arch/crt0.lib lib/lib.lib

LDFLAGS =  -z -L

export CC LIBS DEP AS CFLAGS ASFLAGS LDFLAGS MAKEFLAGS AR CPPFLAGS CPP LD

DIRS  = arch init kernel lib drivers/char


.PHONY: all clean dep distclean


all dep:
	@for i in $(DIRS) ; do make -C $$i $@ || exit 1 ; done



arch/head.obj:
	make -C arch head.obj

%.lib:
	make -C `dirname $@` `basename $@`




distclean: clean
	find . -name "*.d" -exec rm \{\} \;
	rm -f TAGS
	@for i in $(DIRS) ; do make -C $$i $@; done


clean:
	rm -f image l.obj system.map make_tmp
	rm -f `find . -name "*.c" | sed 's/\.c/\.as/'`
	find . -name make_tmp -exec \{\} \;
	@for i in $(DIRS) ; do make -C $$i $@; done


l.obj:  $(LIBS) arch/head.obj
	$(LD) -z -Ptext=100h/0,data,bss -MSystem.map arch/head.obj $(LIBS)


image: l.obj
	$(OBJ2HEX) -b $< image
	rm -f l.obj
