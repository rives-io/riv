# Auto detect toolchain to use
HOST_ARCH=$(shell uname -m)
ifeq ($(HOST_ARCH),riscv64)
	TOOLCHAIN_PREFIX?=
	CC=$(TOOLCHAIN_PREFIX)gcc
	CXX=$(TOOLCHAIN_PREFIX)g++
else
	CC=riscv64-linux-musl-gcc
	CXX=riscv64-linux-musl-g++
	TOOLCHAIN_PREFIX=$(shell $(CC) -dumpmachine)-
	DEPS=../../libriv/riv.h ../../libriv/libriv.so
	ifeq ($(DEMOLANG),nelua)
		DEPS+=../../libriv/riv.nelua
		NELUA_FLAGS+=-L../../libriv/?.nelua
	endif
endif

OBJDUMP=$(TOOLCHAIN_PREFIX)objdump
READELF=$(TOOLCHAIN_PREFIX)readelf
SIZE=$(TOOLCHAIN_PREFIX)size
STRIP=$(TOOLCHAIN_PREFIX)strip
STRINGS=$(TOOLCHAIN_PREFIX)strings
SSTRIP=sstrip
ELFTOC=elftoc
OBJDUMP_FLAGS+=--all-headers --disassemble --disassemble-zeroes
COMP?=xz
ASSETS_DIR?=assets

# All of the following flags are known to help generating the small RISC-V ELF binaries
CFLAGS+=-Os -ffast-math -DNDEBUG
CFLAGS+=-ffunction-sections -fdata-sections
CFLAGS+=-fno-stack-protector -fno-unwind-tables -fno-asynchronous-unwind-tables
CFLAGS+=-mstrict-align -mshorten-memrefs -mno-riscv-attribute
CFLAGS+=-Wall
STRIPFLAGS+=--strip-all --strip-unneeded
STRIPFLAGS+=--remove-section=.note --remove-section=.comment
STRIPFLAGS+=--remove-section=.riscv.attributes --remove-section=.eh_frame
LDFLAGS+=-Wl,-O1,--gc-sections,--as-needed,--no-eh-frame-hdr,--build-id=none,--hash-style=gnu
LDFLAGS+=-Wl,--relax,--sort-common,--sort-section=name
LDFLAGS+=-Wl,--dynamic-linker=/lib/ld-musl.so
LDFLAGS+=-z norelro -z noseparate-code -z lazy

# Add RIV library (required when cross compiling)
CFLAGS+=-I../../libriv
LDFLAGS+=-L../../libriv -lriv
ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

ifneq ($(NAME),)
all: build/$(NAME).sqfs build/$(NAME).dump.txt build/$(NAME).readelf.txt build/$(NAME).strings.txt build/$(NAME).hash.txt build/$(NAME).elftoc.c

run: build/$(NAME).sqfs
	cd ../.. && ./rivemu/rivemu -cartridge=$(ROOT_DIR)/build/$(NAME).sqfs $(ARGS)
endif

clean:
	rm -rf build

build/%.sqfs: build/%.fs
	mksquashfs $< $@ \
	    -quiet \
	    -comp $(COMP) \
	    -mkfs-time 0 \
	    -all-time 0 \
	    -all-root \
	    -noappend \
	    -no-fragments \
	    -no-exports \
	    -no-progress
# 	genext2fs $@ --root $< \
# 		--faketime \
# 		--block-size 4096 \
# 		--readjustment +1 \
# 		--squash-uids 0

build:
	mkdir -p build

build/%.fs: build/%.min.elf
	rm -rf $@
	mkdir -p $@
	if [ -d $(ASSETS_DIR) ]; then cp -r $(ASSETS_DIR)/* $@/; fi
	cp $< $@/$*

build/%.min.elf: build/%.elf
	cp $< $@
	$(STRIP) $(STRIPFLAGS) $@
	$(SSTRIP) $@

ifeq ($(DEMOLANG),nelua)
NELUA_FLAGS+=-Pnochecks -Pnoerrorloc -Pnocstaticassert -Pnogc --verbose
#NELUA_FLAGS+=-Pnocfeaturessetup -Pnocwarnpragmas
build/%.elf: %.nelua $(DEPS) | build
	nelua $(NELUA_FLAGS) \
		--cc=$(CC) \
		--cflags="$(CFLAGS)" \
		--ldflags="$(LDFLAGS)" \
		--cache-dir build \
		--binary \
		--output $@ $<
	touch $@
else ifeq ($(DEMOLANG),c)
build/%.elf: $(OBJS) | build
	$(CC) $(LDFLAGS) $(OBJS) -o $@

build/%.o: %.c | build
	$(CC) $(CFLAGS) -c $< -o $@
endif

build/%.dump.txt: build/%.elf
	$(OBJDUMP) $(OBJDUMP_FLAGS) $< > $@

build/%.readelf.txt: build/%.elf
	$(READELF) -a $< > $@

build/%.strings.txt: build/%.min.elf
	$(STRINGS) $< > $@

build/%.elftoc.c: build/%.min.elf
	$(ELFTOC) $< > $@

build/%.hash.txt: build/%.min.elf build/%.sqfs
	stat -c "%s %n" $^ > $@
	sha1sum $^ >> $@

.PHONY: all run clean
.PRECIOUS: build/%.elf build/%.fs
