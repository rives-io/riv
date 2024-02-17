PREFIX=/usr/local

# Auto detect toolchain to use
HOST_ARCH=$(shell uname -m)
ifeq ($(HOST_ARCH),riscv64)
	TOOLCHAIN_PREFIX=
	CC=$(TOOLCHAIN_PREFIX)gcc
else
	CC=riscv64-buildroot-linux-musl-gcc
	TOOLCHAIN_PREFIX=$(shell $(CC) -dumpmachine)-
endif

# Release flags
CFLAGS+=-Wall
LDFLAGS+=-Wl,-O1,--sort-common,--build-id=none,--hash-style=gnu
LDFLAGS+=-Wl,--relax,--sort-common,--sort-section=name
LDFLAGS+=-Wl,--dynamic-linker=/lib/ld-musl.so
LDFLAGS+=-z relro -z now
NELUA_FLAGS+=-Pnochecks -Pnoerrorloc -Pnocstaticassert --verbose

ifeq ($(debug), yes)
	CFLAGS+=-Og
else
	CFLAGS+=-O2 -DNDEBUG -fno-stack-protector
	STRIPFLAGS+=--strip-unneeded --remove-section=.comment
	NELUA_FLAGS+=--strip-bin --stripflags="$(STRIPFLAGS)"
endif

DEPS=*.nelua \
	$(wildcard ../../libs/guest-host/*.{h,c,nelua}) \
	$(wildcard ../../libs/host/*.{h,c,nelua}) \
	../../libriv/riv_types.nelua \
	../../libriv/riv_log.nelua \
	../../libriv/riv.h

riv-unpacklog: riv-unpacklog.nelua $(DEPS)
	nelua $(NELUA_FLAGS) \
		--cc=$(CC) \
		--cflags="$(CFLAGS)" \
		--ldflags="$(LDFLAGS)" \
		--cache-dir . \
		--output $@ $<
	touch $@

clean:
	rm -f riv-unpacklog riv-unpacklog.c

install: riv-unpacklog
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m755 riv-unpacklog $(DESTDIR)$(PREFIX)/bin/riv-unpacklog
