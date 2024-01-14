PREFIX=/usr/local

# Auto detect toolchain to use
HOST_ARCH=$(shell uname -m)
ifeq ($(HOST_ARCH),riscv64)
	TOOLCHAIN_PREFIX=
	CC=$(TOOLCHAIN_PREFIX)gcc
else
	CC=riscv64-linux-musl-gcc
	TOOLCHAIN_PREFIX=$(shell $(CC) -dumpmachine)-
endif

# Release flags
CFLAGS+=-Wall
LDFLAGS+=-Wl,-O1,--sort-common,--build-id=none,--hash-style=gnu
LDFLAGS+=-Wl,--relax,--sort-common,--sort-section=name
LDFLAGS+=-z relro -z now -rdynamic
NELUA_FLAGS+=-Pnochecks -Pnoerrorloc -Pnocstaticassert --verbose

ifeq ($(debug), yes)
	CFLAGS+=-Og
else
	CFLAGS+=-O2 -DNDEBUG -fno-stack-protector
	STRIPFLAGS+=--strip-unneeded --remove-section=.comment
	NELUA_FLAGS+=--strip-bin --stripflags="$(STRIPFLAGS)"
endif

all: libriv.so

libriv.so: libriv.nelua *.nelua
	nelua $(NELUA_FLAGS) \
		--shared-lib \
		--cc=$(CC) \
		--cflags="$(CFLAGS)" \
		--ldflags="$(LDFLAGS)" \
		--cache-dir . \
		--output $@ $<
	touch $@

update-bindings:
	nelua-lua genbindings.lua

clean:
	rm -f libriv.so libriv.c

install: libriv.so
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	install -m755 libriv.so $(DESTDIR)$(PREFIX)/lib/libriv.so.0.2
	ln -sf libriv.so.0.2 $(DESTDIR)$(PREFIX)/lib/libriv.so.0
	ln -sf libriv.so.0 $(DESTDIR)$(PREFIX)/lib/libriv.so

install-dev:
	mkdir -p $(DESTDIR)$(PREFIX)/include
	install -m644 riv.h $(DESTDIR)$(PREFIX)/include/riv.h

	mkdir -p $(DESTDIR)$(PREFIX)/lib/nelua/lib
	install -m644 riv.nelua $(DESTDIR)$(PREFIX)/lib/nelua/lib/riv.nelua
	install -m644 riv_types.nelua $(DESTDIR)$(PREFIX)/lib/nelua/lib/riv_types.nelua
