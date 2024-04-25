NAME=hello
CARTRIDGE=hello-nelua.sqfs
DATA_FILES=
COMP=xz
RIVEMU_RUN=rivemu
RIVEMU_EXEC=rivemu -quiet -no-window -sdk -workspace -exec
ifneq (,$(wildcard /usr/sbin/riv-run))
	RIVEMU_RUN=riv-run
	RIVEMU_EXEC=
endif
CFLAGS=$(shell $(RIVEMU_EXEC) riv-opt-flags -Ospeed)

build: $(CARTRIDGE)

run: $(CARTRIDGE)
	$(RIVEMU_RUN) $<

clean:
	rm -f *.sqfs *.elf *.c

$(CARTRIDGE): $(NAME).elf $(DATA_FILES)
	$(RIVEMU_EXEC) riv-mksqfs $^ $@ -comp $(COMP)

$(NAME).elf: $(NAME).nelua *.nelua
	$(RIVEMU_EXEC) nelua --verbose --release --binary --cache-dir=. --cflags="$(CFLAGS)" --output=$@ $<
	$(RIVEMU_EXEC) riv-strip $@
