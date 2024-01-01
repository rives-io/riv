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
CFLAGS+=-O2 -DNDEBUG -Wall
LDFLAGS+=-Wl,-O1,--sort-common,--build-id=none,--hash-style=gnu
LDFLAGS+=-Wl,--relax,--sort-common,--sort-section=name
LDFLAGS+=-Wl,--dynamic-linker=/lib/ld-musl.so
LDFLAGS+=-z relro -z now -s
NELUA_FLAGS+=-Pnochecks -Pnoerrorloc -Pnocstaticassert --release --verbose

riv-unpacklog: riv-unpacklog.nelua ../../libs/guest-host/* ../../libriv/*
	nelua $(NELUA_FLAGS) \
		--cc=$(CC) \
		--cflags="$(CFLAGS)" \
		--ldflags="$(LDFLAGS)" \
		--cache-dir . \
		--output $@ $<

clean:
	rm -f riv-unpacklog riv-unpacklog.c

install: riv-unpacklog
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m755 riv-unpacklog $(DESTDIR)$(PREFIX)/bin/riv-unpacklog
