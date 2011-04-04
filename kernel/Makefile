


OBJS = sys.obj panic.obj
DEPS = $(OBJS:.obj=.d)


all: kernel.lib

clean:
	rm -f *.obj *.lib

kernel.lib:  $(OBJS)

include Makefile.rules
### Dependencies
