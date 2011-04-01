
OBJS = main.obj
DEPS = $(OBJS:.obj=.d)

all: init.lib


clean:
	rm -f *.obj *.lib

init.lib:  $(OBJS)
	$(AR) r $@ $?

include Makefile.rules
### Dependencies:
main.o: main.c include/arch.h include/sys.h

include/arch.h:

include/sys.h:
