LIBRARY=libingle.a
SOURCES= \
    ingle.asm \
    launch.asm \
    save-state.asm \
    zeropage.asm

include ../../Makefile.rules

%.o: %.asm
	$(AS) -g -o $@ $<

$(LIBRARY): $(SOURCES:.asm=.o)
	$(AR) a $@ $^

all: $(LIBRARY)

clean:
	rm -f $(SOURCES:.asm=.o) $(LIBRARY)
