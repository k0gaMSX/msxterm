
OBJS = machine.obj
DEPS = $(OBJS:.obj=.d)

all: head.obj arch.lib


arch.lib: $(OBJS)
	$(AR) r $@ $?


clean:
	rm -f *.obj *.lib


include Makefile.rules
### Dependencies:
machine.o: machine.c include/arch.h

include/arch.h:
