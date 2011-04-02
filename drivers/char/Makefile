
OBJS = bell.obj keyboard.obj tty.obj vt.obj kbd.obj
DEPS = $(OBJS:.obj=.d)

all: char.lib

clean:
	rm -f *.obj *.lib

char.lib:  $(OBJS)
	$(AR) r $@ $?


include Makefile.rules
### Dependencies:
