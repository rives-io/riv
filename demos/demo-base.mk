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
STRIP=$(TOOLCHAIN_PREFIX)strip
STRINGS=$(TOOLCHAIN_PREFIX)strings
SSTRIP=sstrip
ELFTOC=elftoc
OBJDUMP_FLAGS=--all-headers --disassemble --disassemble-zeroes

# All of the following flags are known to help generating the small RISC-V ELF binaries
CFLAGS+=-Os -ffast-math -DNDEBUG -flto=auto
CFLAGS+=-ffunction-sections -fdata-sections
CFLAGS+=-fno-stack-protector -fno-unwind-tables -fno-asynchronous-unwind-tables
CFLAGS+=-mstrict-align -mshorten-memrefs -mno-riscv-attribute
CFLAGS+=-Wall
STRIPFLAGS+=--strip-all --strip-unneeded
STRIPFLAGS+=--remove-section=.note --remove-section=.comment
STRIPFLAGS+=--remove-section=.riscv.attributes --remove-section=.eh_frame
LDFLAGS+=-Wl,-O1,--gc-sections,--as-needed,--no-eh-frame-hdr,--build-id=none,--hash-style=gnu
LDFLAGS+=-Wl,--relax,--sort-common,--sort-section=name,--spare-dynamic-tags=0
LDFLAGS+=-Wl,--dynamic-linker=/lib/ld.so
LDFLAGS+=-z norelro -z noseparate-code -z lazy

# Add RIV library (required when cross compiling)
CFLAGS+=-I../../libriv
LDFLAGS+=-L../../libriv -lriv

all: \
	$(NAME).sqfs \
	$(NAME).elf \
	$(NAME).min.elf \
	$(NAME).dump.txt \
	$(NAME).readelf.txt \
	$(NAME).strings.txt \
	$(NAME).elftoc.c \
	$(NAME).hash.txt

$(NAME).fs: $(NAME).min.elf
	rm -rf $@
	mkdir -p $@
	if [ -d assets ]; then cp -r assets/* $@/; fi
	cp $< $@/$(NAME)

$(NAME).min.elf: $(NAME).elf
	cp $< $@
	$(STRIP) $(STRIPFLAGS) $@
	$(SSTRIP) $@

$(NAME).dump.txt: $(NAME).elf
	$(OBJDUMP) $(OBJDUMP_FLAGS) $< > $@

$(NAME).readelf.txt: $(NAME).elf
	$(READELF) -a $< > $@

$(NAME).strings.txt: $(NAME).min.elf
	$(STRINGS) $< > $@

$(NAME).elftoc.c: $(NAME).min.elf
	$(ELFTOC) $< > $@

$(NAME).hash.txt: $(NAME).min.elf $(NAME).sqfs
	stat -c "%s %n" $^ > $@
	sha1sum $^ >> $@

$(NAME).sqfs: $(NAME).fs
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
# 	genext2fs $@ --root $< \
# 		--faketime \
# 		--block-size 4096 \
# 		--readjustment +1 \
# 		--squash-uids 0

run: $(NAME).sqfs
	cd ../.. && ./rivcm/rivcm -cartridge=demos/$(NAME)/$(NAME).sqfs $(ARGS)

clean::
	rm -f *.o *.txt *.elf *.sqfs *.elftoc.c
	rm -rf *.fs

.PHONY: run clean
.PRECIOUS: %.elf $(NAME).fs
