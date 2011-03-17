MSXTERM_OBJS = 	bell.obj kbd.obj vt100.obj \
		vram.obj main.obj bell.obj \
		v9990.obj hitech.obj

CONSOLECHARS_OBJS = conchar.obj vram.obj v9990.obj vram.obj hitech.obj

BIN = msxterm.com conchar.com

DEPS = $(MSXTERM_OBJS:.obj=.d)
DEPS += $(CONSOLECHARS_OBJS:.obj=.d)

LIBS =
DEP = gcc
CC = zc
AS = zas
CFLAGS = -CPM -Zg3 -W-3 -P8
ASFLAGS =
LDFLAGS = -CPM

all: $(BIN)

conchar.com: $(CONSOLECHARS_OBJS)
	$(CC)  $(LDFLAGS) $^ $(LIBS) -O$@


msxterm.com:  $(MSXTERM_OBJS)
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
	rm -f TAGS
	rm -f *.crf
	rm -f $(BIN)



include $(DEPS)
