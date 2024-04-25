DATA_FILES=info.json sprites.png
RIVEMU=rivemu
RIVEMU_EXEC=$(RIVEMU) -quiet -no-window -sdk -workspace -exec
RIVEMU_RUN=$(RIVEMU) -quiet -workspace -exec
CFLAGS=$(shell $(RIVEMU_EXEC) riv-opt-flags -Ospeed)

# Compile all cartridges
all: snake-1.sqfs snake-2.sqfs snake-3.sqfs snake-4.sqfs snake-5.sqfs snake-6.sqfs snake-7.sqfs snake-8.sqfs snake-8.sqfs snake-9.sqfs

# Compile cartridge
snake-%.elf: snake-%.c
	$(RIVEMU_EXEC) "gcc $< -o $@ $(CFLAGS) && riv-strip $@"

# Compile final cartridge file
snake-%.sqfs: snake-%.elf $(DATA_FILES)
	$(RIVEMU_EXEC) "riv-mksqfs $(DATA_FILES) $< $@"

# Test all chapters
test: all
	$(RIVEMU_RUN) riv-jit-c snake-1.c
	$(RIVEMU_RUN) riv-jit-c snake-2.c
	$(RIVEMU_RUN) riv-jit-c snake-3.c
	$(RIVEMU_RUN) riv-jit-c snake-4.c
	$(RIVEMU_RUN) riv-jit-c snake-5.c
	$(RIVEMU_RUN) riv-jit-c snake-6.c
	$(RIVEMU_RUN) riv-jit-c snake-7.c
	$(RIVEMU_RUN) riv-jit-c snake-8.c
	$(RIVEMU_RUN) riv-jit-c snake-9.c

# Check code for syntax errors
lint:
	gcc -fsyntax-only -fanalyzer -I../../libriv *.c
	clang-tidy *.c -- -I../../libriv

# Clean generated cartridges
clean:
	rm -f *.sqfs *.elf
