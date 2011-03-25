
DEP 	= gcc
CC 	= zc
AS 	= zas
CPPFLAGS = -Iinclude
CFLAGS 	= -CPM -Zg3 -W-3 -P8 $(CPPFLAGS)
ASFLAGS =
MAKEFLAGS = --include-dir=$(PWD)
AR	= libr r

export CC LIBS DEP AS CFLAGS ASFLAGS LDFLAGS MAKEFLAGS AR CPPFLAGS


DIRS    = kernel drivers/char lib
LIBS 	= kernel/kernel.lib drivers/char/char.lib lib/lib.lib
LDFLAGS = -CPM $(LIBS)


.PHONY: all clean dep distclean

all clean dep distclean:
	for i in $(DIRS) ; do \
		make -C $$i $@ ;\
	done

image: all
	$(CC) $(LDFLAGS)  -O$@
