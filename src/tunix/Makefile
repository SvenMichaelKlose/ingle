PROGRAM=tunix.prg
ASMSOURCES= \
	main.asm
#SOURCES= \
#	syscalls.c \
#	main.c

include ../Makefile.rules

%.o: %.asm
	$(AS) -o $@ $<

%.o: %.c
	$(CC) -c $(CFLAGS) -I../lib/ -o $@ $<

$(PROGRAM): $(SOURCES:.c=.o) $(ASMSOURCES:.asm=.o)
	$(LD) -C vic.cfg -Ln $(PROGRAM).lbl -o $@ $^ ../lib/term/libterm.a ../lib/gfx/libgfx.a ../lib/blit/libblit.a ../lib/ingle/libingle.a ../lib/ultimem/libultimem.a $(CC65_HOME)/share/cc65/lib/vic20.lib

tunix: $(PROGRAM)
	exomizer-3.0.2 sfx basic $(PROGRAM) -t52 -o tunix

all: tunix

clean:
	rm -f $(ASMSOURCES:.asm=.o) $(SOURCES:.c=.o) $(PROGRAM) $(PROGRAM).lbl tunix
