MSXTERM_SRC = bell.c kbd.c vt100.c vram.c main.c encoding.c kbd.c tty.c
MSXTERM_OBJ = $(MSXTERM_SRC:.c=.obj)

SHOWKEY_SRC = kbd.c showkey.c
SHOWKEY_OBJ = $(SHOWKEY_SRC:.c=.obj)

SRC = $(MSXTERM_SRC) $(SHOWKEY_SRC)
DEPS = $(SRC:.c=.d)
LIBS =
DEP = gcc
CC = zc
CFLAGS = -CPM
TARGETS = msxterm.com showkey.com


all: $(TARGETS)

install: all
	sb $(TARGETS)

showkey.com:  $(SHOWKEY_OBJ)
	$(CC) $(CFLAGS) $^ $(LIBS) -O$@


msxterm.com:  $(MSXTERM_OBJ)
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
	rm -f TAGS
	rm -f *.com



include $(DEPS)
