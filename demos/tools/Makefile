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

lint:
	gcc -Wall -Wextra -fsyntax-only -fanalyzer -I../../libriv *.c
	clang-tidy *.c -- -I../../libriv

gamepad.sqfs: DATA_FILES=gamepad.png
%.sqfs: %.elf
	$(RIVEMU_EXEC) riv-mksqfs $^ $(DATA_FILES) $@ -comp $(COMP)

%.elf: %.c
	$(RIVEMU_EXEC) "gcc $< -o $@ $(CFLAGS) && riv-strip $@"
