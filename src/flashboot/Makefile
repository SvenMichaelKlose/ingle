PROGRAM=flashboot.bin
ASMSOURCES=alloc.asm api.asm copy-bank.asm launch.asm main.asm save-state.asm rom-info.asm prgend.asm

include ../Makefile.rules

%.o: %.asm
	$(AS) -o $@ $<

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(PROGRAM): $(SOURCES:.c=.o) $(ASMSOURCES:.asm=.o)
	$(LD) --config rom.cfg -m $(PROGRAM).map -Ln $(PROGRAM).lbl -o $@ $^ ../lib/term/libterm.a ../lib/gfx/libgfx.a ../lib/ingle/libingle.a ../lib/ultifs/libultifs.a ../lib/ultimem/libultimem.a ../lib/blit/libblit.a ../lib/zp/libzp.a

all: $(PROGRAM)

clean:
	rm -f $(ASMSOURCES:.asm=.o) $(SOURCES:.c=.o) $(PROGRAM) $(PROGRAM).map $(PROGRAM).lbl ultifs.bin desktop.bin
