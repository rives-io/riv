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
NELUA_FLAGS+=-Pnochecks -Pnoassertloc -Pnocstaticassert --release --verbose

all: bwrapbox seccomp-filter seccomp-filter.bpf

bwrapbox: bwrapbox.nelua
	nelua $(NELUA_FLAGS) \
		--cc=$(CC) \
		--cflags="$(CFLAGS)" \
		--ldflags="$(LDFLAGS)" \
		--cache-dir . \
		--output $@ $<

seccomp-filter: seccomp-filter.c seccomp-filter-rules.h
	$(CC) -o $@ $< -lseccomp

seccomp-filter.bpf: seccomp-filter
	./seccomp-filter $@

clean:
	rm -f bwrapbox seccomp-filter seccomp-filter.bpf bwrapbox.c

install: bwrapbox seccomp-filter.bpf
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m755 bwrapbox $(DESTDIR)$(PREFIX)/bin/bwrapbox
	mkdir -p $(DESTDIR)$(PREFIX)/lib/bwrapbox
	install -m644 seccomp-filter.bpf $(DESTDIR)$(PREFIX)/lib/bwrapbox/seccomp-filter.bpf
