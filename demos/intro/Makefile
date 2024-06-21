NAME=intro
DATA_FILES=logo.png
COMP=xz
RIVEMU=rivemu
RIVEMU_RUN=$(RIVEMU)
RIVEMU_EXEC=$(RIVEMU) -quiet -no-window -sdk -workspace -exec
ifneq (,$(wildcard /usr/sbin/riv-run))
	RIVEMU_RUN=riv-run
	RIVEMU_EXEC=
endif
LIBRIV_PATH=libriv
CFLAGS+=-Ilibriv -Llibriv
CROSS=n
ifeq ($(CROSS),y)
	CFLAGS+=-Ilibriv -Llibriv -lriv
	CFLAGS+=-Wall -Wextra
	CFLAGS+=-march=rv64g -Og -g -rdynamic -fno-omit-frame-pointer -fno-strict-overflow -fno-strict-aliasing
	CFLAGS+=-Wl,--build-id=none,--sort-common
	NELUA=nelua
	NELUA_FLAGS=--cc=riscv64-buildroot-linux-musl-gcc --add-path=libriv
else
	NELUA=$(RIVEMU_EXEC) nelua
	NELUA_FLAGS=--release
	CFLAGS=$(shell $(RIVEMU_EXEC) riv-opt-flags -Ospeed)
endif

build: $(NAME).sqfs

run: $(NAME).sqfs
	$(RIVEMU_RUN) $<

screenshot: $(NAME).png

quick-run: $(NAME).elf
	$(RIVEMU_RUN) -no-loading -bench -workspace -exec ./$<

live-dev:
	luamon -e nelua,Makefile -x 'make quick-run'

clean:
	rm -rf *.sqfs *.elf *.c

distclean: clean
	rm -rf libriv

$(NAME).sqfs: $(NAME).elf $(DATA_FILES)
	$(RIVEMU_EXEC) riv-mksqfs $^ $@ -comp $(COMP)

$(NAME).elf: $(NAME).nelua *.nelua libriv
	$(NELUA) \
		$(NELUA_FLAGS) \
		--verbose \
		--binary \
		--cache-dir=. \
		--cflags='$(CFLAGS)' \
		--output=$@ $<

$(NAME).png: $(NAME).sqfs
	$(RIVEMU) -save-screenshot=$(NAME).png -load-incard=songs/03.seqt.rivcard -stop-frame=60 $(NAME).sqfs
	magick $(NAME).png -scale 200% $(NAME).png
	oxipng $(NAME).png

libriv:
	mkdir -p libriv
	$(RIVEMU_EXEC) cp /usr/include/riv.h libriv/
	$(RIVEMU_EXEC) cp /usr/lib/libriv.so libriv/
	$(RIVEMU_EXEC) cp /usr/lib/nelua/lib/riv.nelua libriv/
	$(RIVEMU_EXEC) cp /usr/lib/nelua/lib/riv_types.nelua libriv/
	$(RIVEMU_EXEC) cp /lib/libc.musl-riscv64.so.1 libriv/
