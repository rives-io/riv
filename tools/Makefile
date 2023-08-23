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
CFLAGS=-O2 -fno-stack-protector -Wall -Werror
LDFLAGS=-s -Wl,-O1,--sort-common,--build-id=none -z relro -z now -rdynamic
STRIPFLAGS=--strip-unneeded --remove-section=.comment
NELUA_FLAGS=-Pnochecks -Pnoassertloc -Pnocstaticassert --release --verbose

unpack-rivlog: unpack-rivlog.nelua ../libs/guest-host/* ../libriv/*
	nelua $(NELUA_FLAGS) \
		--cc=$(CC) \
		--cflags="$(CFLAGS)" \
		--binary \
		--ldflags="$(LDFLAGS)" \
		--strip-bin \
		--stripflags="$(STRIPFLAGS)" \
		--cache-dir . \
		--output $@ $<

clean:
	rm -f unpack-rivlog unpack-rivlog.c

install: unpack-rivlog
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m755 unpack-rivlog $(DESTDIR)$(PREFIX)/bin/unpack-rivlog
