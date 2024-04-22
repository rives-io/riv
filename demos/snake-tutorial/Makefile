DATA_FILES=info.json sprites.png
CFLAGS=$(shell rivemu -quiet -no-window -sdk -workspace -exec riv-opt-flags -Ospeed)

# Compile all cartridges
all: snake-1.sqfs snake-2.sqfs snake-3.sqfs snake-4.sqfs snake-5.sqfs snake-6.sqfs snake-7.sqfs snake-8.sqfs snake-8.sqfs snake-9.sqfs

# Compile cartridge
snake-%.elf: snake-%.c
	rivemu -quiet -no-window -sdk -workspace -exec "gcc $< -o $@ $(CFLAGS) && riv-strip $@"

# Compile final cartridge file
snake-%.sqfs: snake-%.elf $(DATA_FILES)
	rivemu -quiet -no-window -sdk -workspace -exec "riv-mksqfs $(DATA_FILES) $< $@"

# Test all chapters
test:
	rivemu -quiet -workspace -exec riv-jit-c ./snake-1.c
	rivemu -quiet -workspace -exec riv-jit-c ./snake-2.c
	rivemu -quiet -workspace -exec riv-jit-c ./snake-3.c
	rivemu -quiet -workspace -exec riv-jit-c ./snake-4.c
	rivemu -quiet -workspace -exec riv-jit-c ./snake-5.c
	rivemu -quiet -workspace -exec riv-jit-c ./snake-6.c
	rivemu -quiet -workspace -exec riv-jit-c ./snake-7.c
	rivemu -quiet -workspace -exec riv-jit-c ./snake-8.c
	rivemu -quiet -workspace -exec riv-jit-c ./snake-9.c

# Check code for syntax errors
lint:
	gcc -fsyntax-only -fanalyzer -I../../libriv *.c
	clang-tidy *.c -- -I../../libriv

# Clean generated cartridges
clean:
	rm -f *.sqfs *.elf
