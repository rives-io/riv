CC=gcc
SSTRIP=sstrip
OBJDUMP=objdump
READELF=readelf
SIZE=size
STRINGS=strings
ELFTOC=elftoc
OBJDUMP_FLAGS=--all-headers --disassemble --disassemble-zeroes

# All of the following flags are known to help generating the small RISC-V ELF binaries
NELUA_FLAGS+=-Pnochecks -Pnoassertloc -Pnocstaticassert --release --verbose
CFLAGS+=-Os -DNDEBUG -ffast-math -flto=auto
CFLAGS+=-ffunction-sections -fdata-sections
CFLAGS+=-fno-stack-protector -fno-unwind-tables -fno-asynchronous-unwind-tables
CFLAGS+=-mno-save-restore -mstrict-align -mshorten-memrefs -msmall-data-limit=0
CFLAGS+=-mno-plt -fno-plt -fpie -pie -fno-semantic-interposition
STRIPFLAGS+=--strip-unneeded --remove-section=.note --remove-section=.comment
STRIPFLAGS+=--remove-section=.eh_frame --remove-section=.eh_frame_hdr
STRIPFLAGS+=--remove-section=.riscv.attributes
LDFLAGS+=-Wl,-O1,-gc-sections,--as-needed,--no-eh-frame-hdr,--build-id=none,--hash-style=sysv
LDFLAGS+=-Wl,--relax,--sort-common,--sort-section=alignment,--spare-dynamic-tags=0
LDFLAGS+=-Wl,--dynamic-linker=/lib/ld.so
LDFLAGS+=-z norelro -z noseparate-code -z lazy

all: snake.sqfs

%.elf: %.nelua ../libriv/*.nelua ../libriv/*.h
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
	    -noappend \
	    -comp xz \
	    -no-fragments \
	    -mkfs-time 0 \
	    -all-time 0 \
	    -all-root \
	    -no-progress
	stat -c "%s %n" $*.elf $@ > $*.hash.txt
	sha1sum $*.elf $@ >> $*.hash.txt
	cat $*.hash.txt

clean:
	rm -f *.c *.txt *.elf *.sqfs
	rm -rf *.fs

.PRECIOUS: %.elf %.fs
