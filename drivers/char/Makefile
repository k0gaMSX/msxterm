
OBJS = bell.obj keyboard.obj tty.obj vram.obj vt100.obj kbd.obj v9990.obj
DEPS = $(OBJS:.obj=.d)

all: char.lib


char.lib:  $(OBJS)
	for i in $? ; do \
		$(AR) $@ $$i ;\
	done

include Makefile.rules
### Dependencies:
