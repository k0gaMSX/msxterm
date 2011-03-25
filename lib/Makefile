OBJS = printk.obj
DEPS = $(OBJS:.obj=.d)


all: lib.lib


lib.lib:  $(OBJS)
	for i in $? ; do \
		$(AR) $@ $$i ;\
	done



include Makefile.rules
### Dependencies:
printk.o: printk.c
