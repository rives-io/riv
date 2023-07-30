CC=gcc
SSTRIP=sstrip
OBJDUMP=objdump
READELF=readelf
SIZE=size
STRINGS=strings
ELFTOC=elftoc

CFLAGS=
NELUA_FLAGS=-Pnogc -Pnochecks -Pnoassertloc -Pnocstaticassert --release --verbose
NELUA_LIBS=-L../libriv
OBJDUMP_FLAGS=--all-headers --disassemble --disassemble-zeroes
STRIPFLAGS=--strip-unneeded \
	--remove-section=.note \
	--remove-section=.note.gnu.build-id \
	--remove-section=.note.ABI-tag \
	--remove-section=.comment \
	--remove-section=.eh_frame \
	--remove-section=.eh_frame_hdr \
	--remove-section=.riscv.attributes

all: snake.sqfs

%.elf: %.nelua ../libriv/*.nelua ../libriv/*.h
	nelua $(NELUA_FLAGS) $(NELUA_LIBS) \
		--cc=$(CC) \
		--cflags="$(CFLAGS)" \
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
