PROGRAM=desktop-lib.bin
SOURCES= \
    desktop.c \
    file-window.c \
    main.c \
    save-state.c \
    wrap-file-window.c
ASMSOURCES= \
    prgend.asm

include ../Makefile.rules

%.o: %.asm
	$(AS) -o $@ $<

%.o: %.c
	$(CC) --register-space 19 -O -c $(CFLAGS) -I $(CC65_INCLUDE) -I.. -o $@ $<

$(PROGRAM): $(SOURCES:.c=.o) $(ASMSOURCES:.asm=.o)
	$(LD) -C vic20.cfg -m $@.map -Ln $@.lbl -o $@ $^ --lib ../lib/gui/libgui.a --lib ../lib/gfx/libgfx.a --lib ../lib/ultimem/libultimem.a --lib ../lib/ingle/libingle.a --lib ../lib/blit/libblit.a --lib $(CC65_HOME)/share/cc65/lib/vic20.lib

all: $(PROGRAM)
	$(AS) -o charset-4x8.o charset-4x8-light.asm
	$(LD) -C charset.cfg -o charset-4x8.bin charset-4x8.o

clean:
	rm -f $(ASMSOURCES:.asm=.o) $(SOURCES:.c=.o) $(PROGRAM) $(PROGRAM).lbl $(PROGRAM).map desktop.bin charset-4x8.o charset-4x8.bin desktop-file-window.bin
