
DEP 	= gcc
CC 	= zc
AS 	= zas
CPP     = cpp
CPPFLAGS = -Iinclude
CFLAGS 	= -Zg3 -W-2 -P8 -BS $(CPPFLAGS)
ASFLAGS =
MAKEFLAGS = --include-dir=$(PWD)
AR	= libr
OBJ2HEX = objtohex
LD	= hlink


LIBS 	=  init/init.lib kernel/kernel.lib arch/arch.lib

LDFLAGS =  -z -L

export CC LIBS DEP AS CFLAGS ASFLAGS LDFLAGS MAKEFLAGS AR CPPFLAGS CPP LD

DIRS  = arch init kernel


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


clean:
	rm -f image l.obj system.map make_tmp
	rm -f `find . -name "*.c" | sed 's/\.c/\.as/'`
	find . -name make_tmp -exec \{\} \;
	@for i in $(DIRS) ; do make -C $$i $@; done


l.obj:  $(LIBS) arch/head.obj
	$(LD) -z -Ptext=100h/0,data,bss -MSystem.map arch/head.obj  $(LIBS)


image: l.obj
	$(OBJ2HEX) -b $< image
	rm -f l.obj
