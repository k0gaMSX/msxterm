


OBJS = sys.obj
DEPS = $(OBJS:.obj=.d)


all: kernel.lib

clean:
	rm -f *.obj *.lib

kernel.lib:  $(OBJS)
	$(AR) r $@ $?


include Makefile.rules
### Dependencies
sys.o: sys.c include/sys.h

include/sys.h:
