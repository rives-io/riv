# Auto detect toolchain to use
HOST_ARCH=$(shell uname -m)
ifeq ($(HOST_ARCH),riscv64)
	TOOLCHAIN_PREFIX=
	CC=$(TOOLCHAIN_PREFIX)gcc
else
	CC=riscv64-linux-musl-gcc
	TOOLCHAIN_PREFIX=$(shell $(CC) -dumpmachine)-
endif

OBJDUMP=$(TOOLCHAIN_PREFIX)objdump
READELF=$(TOOLCHAIN_PREFIX)readelf
SIZE=$(TOOLCHAIN_PREFIX)size
STRINGS=$(TOOLCHAIN_PREFIX)strings
SSTRIP=sstrip
ELFTOC=elftoc
OBJDUMP_FLAGS=--all-headers --disassemble --disassemble-zeroes

# All of the following flags are known to help generating the small RISC-V ELF binaries
NELUA_FLAGS+=-Pnochecks -Pnoassertloc -Pnocstaticassert --release --verbose
CFLAGS+=-Os -DNDEBUG -ffast-math -flto=auto
CFLAGS+=-ffunction-sections -fdata-sections
CFLAGS+=-fno-stack-protector -fno-unwind-tables -fno-asynchronous-unwind-tables
CFLAGS+=-mstrict-align -mshorten-memrefs -mno-riscv-attribute
STRIPFLAGS+=--strip-unneeded --remove-section=.note --remove-section=.comment
STRIPFLAGS+=--remove-section=.riscv.attributes --remove-section=.eh_frame
LDFLAGS+=-Wl,-O1,--gc-sections,--as-needed,--no-eh-frame-hdr,--build-id=none,--hash-style=gnu
LDFLAGS+=-Wl,--relax,--sort-common,--sort-section=alignment,--spare-dynamic-tags=0
LDFLAGS+=-Wl,--dynamic-linker=/lib/ld.so
LDFLAGS+=-z norelro -z noseparate-code -z lazy

# Add RIV library path (required when cross compiling)
NELUA_FLAGS+=-L '../../libriv/?.nelua'
CFLAGS+=-L '../../libriv'

all: $(DEMO_NAME).sqfs

%.elf: %.nelua *.nelua ../../libriv/*.nelua ../../libriv/*.h
	nelua $(NELUA_FLAGS) \
		--cc=$(CC) \
		--cflags="$(CFLAGS)" \
		--ldflags="$(LDFLAGS)" \
		--strip-bin \
		--stripflags="$(STRIPFLAGS)" \
		--cache-dir . \
		--binary \
		--output $@ $<
	$(OBJDUMP) $(OBJDUMP_FLAGS) $@ > $*.dump.txt
	$(READELF) -a $@ > $*.readelf.txt
	$(SIZE) $@ >> $*.readelf.txt
	$(SSTRIP) $@
	$(STRINGS) $@ > $*.strings.txt
	$(ELFTOC) $@ > $*.elftoc.c

%.fs: %.elf
	rm -rf $@
	mkdir -p $@
	cp $< $@/a

%.sqfs: %.fs
	mksquashfs $< $@ \
	    -quiet \
	    -comp xz \
	    -mkfs-time 0 \
	    -all-time 0 \
	    -all-root \
	    -nopad \
	    -no-fragments \
	    -no-exports \
	    -no-progress
	stat -c "%s %n" $*.elf $@ > $*.hash.txt
	sha1sum $*.elf $@ >> $*.hash.txt
	cat $*.hash.txt

run:
	cd ../.. && ./rivcm/rivcm -cartridge=demos/$(DEMO_NAME)/$(DEMO_NAME).sqfs $(DEMO_ARGS)

clean:
	rm -f *.c *.txt *.elf *.sqfs
	rm -rf *.fs

.PRECIOUS: %.elf %.fs
