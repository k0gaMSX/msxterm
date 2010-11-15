SRC = bell.c kbd.c vt100.c vram.c main.c bell.c
OBJ = $(SRC:.c=.o)

DEPS = $(SRC:.c=.d)
LIBS =
DEP = gcc

all:	msxterm


msxterm:  $(OBJ)
	$(CC) $^ $(LIBS) -o $@

%.o: %.d

%.d: %.c
	$(DEP) -MM -MP $< -MF $@


.PHONY:  clean distclean deps

deps:	$(DEPS)


distclean: clean
	rm -f *.d

clean:
	rm -f *.o
	rm -f msxterm
	rm -f TAGS



include $(DEPS)
