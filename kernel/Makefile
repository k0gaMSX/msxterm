


OBJS = msxterm.obj
DEPS = $(OBJS:.obj=.d)


all: kernel.lib


kernel.lib:  $(OBJS)
	for i in $? ; do \
		$(AR) $@ $$i ;\
	done


include Makefile.rules
### Dependencies
