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
CFLAGS+=-O2 -DNDEBUG -Wall -fno-stack-protector
LDFLAGS+=-Wl,-O1,--sort-common,--build-id=none,--hash-style=gnu
LDFLAGS+=-Wl,--relax,--sort-common,--sort-section=name
LDFLAGS+=-Wl,--dynamic-linker=/lib/ld-musl.so
LDFLAGS+=-z relro -z now -s

rndaddentropy: rndaddentropy.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm -f rndaddentropy

install: rndaddentropy
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m755 rndaddentropy $(DESTDIR)$(PREFIX)/bin/rndaddentropy
