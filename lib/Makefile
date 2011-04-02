OBJS = ctype.obj printk.obj assert.obj
DEPS = $(OBJS:.obj=.d)


all: lib.lib

clean:
	rm -f *.obj *.lib


lib.lib:  $(OBJS)


include Makefile.rules
### Dependencies:
