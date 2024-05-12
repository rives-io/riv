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
coins.sqfs: DATA_FILES=simple_dungeon_crawler_16x16.png
shapes.sqfs: DATA_FILES=logo.png
%.sqfs: %.elf
	$(RIVEMU_EXEC) riv-mksqfs $^ $(DATA_FILES) $@ -comp $(COMP)

%.elf: %.c
	$(RIVEMU_EXEC) "gcc $< -o $@ $(CFLAGS) && riv-strip $@"
