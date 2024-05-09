SAMPLES=$(patsubst %.c, %.sqfs, $(wildcard *.c))
COMP=xz
RIVEMU_EXEC=rivemu -quiet -no-window -sdk -workspace -exec
ifneq (,$(wildcard /usr/sbin/riv-run))
	RIVEMU_EXEC=
endif
CFLAGS=$(shell $(RIVEMU_EXEC) riv-opt-flags -Osize)

all: $(SAMPLES)

clean:
	rm -f *.sqfs *.elf

%.sqfs: %.elf $(DATA_FILES)
	$(RIVEMU_EXEC) riv-mksqfs $^ $@ -comp $(COMP)

%.elf: %.c
	$(RIVEMU_EXEC) "gcc $< -o $@ $(CFLAGS) && riv-strip $@"
