SRC = bell.c kbd.c vt100.c vram.c main.c bell.c encoding.c
OBJ = $(SRC:.c=.obj)

DEPS = $(SRC:.c=.d)
LIBS =
DEP = gcc
CC = zc
CFLAGS = -CPM

all:	msxterm


msxterm:  $(OBJ)
	$(CC) $(CFLAGS) $^ $(LIBS) -O$@



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
