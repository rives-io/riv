SAMPLES=$(patsubst %.nelua, %.sqfs, $(wildcard *.nelua))
NAME=game2048
COMP=xz
RIVEMU=rivemu
RIVEMU_EXEC=$(RIVEMU) -quiet -no-window -sdk -workspace -exec
CFLAGS=$(shell $(RIVEMU_EXEC) riv-opt-flags -Ospeed)

all: $(SAMPLES)

clean:
	rm -f *.sqfs *.elf *.c

%.sqfs: %.elf $(DATA_FILES)
	$(RIVEMU_EXEC) riv-mksqfs $^ $@ -comp $(COMP)

%.elf: %.nelua
	$(RIVEMU_EXEC) nelua --verbose --release --binary --cache-dir=. --cflags="$(CFLAGS)" --output=$@ $<
	$(RIVEMU_EXEC) riv-strip $@
