SRC = bell.c kbd.c vt100.c vram.c main.c bell.c memory.c
OBJ = $(SRC:.c=.obj) v9990.obj

DEPS = $(SRC:.c=.d)
LIBS =
DEP = gcc
CC = zc
AS = zas
CFLAGS = -CPM
ASFLAGS =
LDFLAGS = -CPM

all:	msxterm


msxterm:  $(OBJ)
	$(CC)  $(LDFLAGS) $^ $(LIBS) -O$@


%.obj: %.as
	$(AS) $(ASFLAGS) -C $<


%.obj: %.c
	$(CC) $(CFLAGS) -C $<


%.d: %.c
	$(DEP) -MM -MP $< -MF $@


.PHONY:  clean distclean deps

deps:	$(DEPS)


distclean: clean
	rm -f *.d

clean:
	rm -f *.obj
	rm -f msxterm
	rm -f TAGS



include $(DEPS)
