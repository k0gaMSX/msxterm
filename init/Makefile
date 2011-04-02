
OBJS = main.obj
DEPS = $(OBJS:.obj=.d)

all: init.lib

clean:
	rm -f *.obj *.lib

init.lib:  $(OBJS)

include Makefile.rules
### Dependencies:
