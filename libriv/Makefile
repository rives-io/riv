PREFIX=/usr/local

RIV_VERSION_MAJMIN=0.3

# Auto detect toolchain to use
HOST_ARCH=$(shell uname -m)
ifeq ($(HOST_ARCH),riscv64)
	TOOLCHAIN_PREFIX=
	CC=$(TOOLCHAIN_PREFIX)gcc
else
	CC=riscv64-buildroot-linux-musl-gcc
	TOOLCHAIN_PREFIX=$(shell $(CC) -dumpmachine)-
endif

# basic flags
CFLAGS+=-Wall
LDFLAGS+=-Wl,-O1,--sort-common,--build-id=none,--hash-style=gnu
LDFLAGS+=-Wl,-z,relro,-z,now -rdynamic -Wl,--defsym=riv=riv_ctx
NELUA_FLAGS+=-Pnochecks -Pnoerrorloc -Pnocstaticassert --verbose
NELUA_CACHEDIR=.

ifeq ($(debug), yes)
	CFLAGS+=-Og -march=rv64g
else # release flags
	CFLAGS+=-Ofast -march=rv64g -DNDEBUG -fno-stack-protector
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
		--cache-dir="$(NELUA_CACHEDIR)" \
		--output $@ $<
	touch $@

update-bindings:
	nelua-lua genbindings.lua

clean:
	rm -f libriv.so libriv.c

install: libriv.so
	mkdir -p $(DESTDIR)$(PREFIX)/lib $(DESTDIR)$(PREFIX)/share/riv
	install -m755 libriv.so $(DESTDIR)$(PREFIX)/lib/libriv.so.$(RIV_VERSION_MAJMIN)
	ln -sf libriv.so.$(RIV_VERSION_MAJMIN) $(DESTDIR)$(PREFIX)/lib/libriv.so.0
	ln -sf libriv.so.0 $(DESTDIR)$(PREFIX)/lib/libriv.so
	ln -sf ../lib/libriv.so $(DESTDIR)$(PREFIX)/bin/riv-tool
	echo $(RIV_VERSION_MAJMIN) > $(DESTDIR)$(PREFIX)/share/riv/version

install-c-dev:
	mkdir -p $(DESTDIR)$(PREFIX)/include
	install -m644 riv.h $(DESTDIR)$(PREFIX)/include/riv.h

install-nelua-dev:
	mkdir -p $(DESTDIR)$(PREFIX)/include
	mkdir -p $(DESTDIR)$(PREFIX)/lib/nelua/lib
	install -m644 riv.nelua $(DESTDIR)$(PREFIX)/lib/nelua/lib/riv.nelua
	install -m644 riv_types.nelua $(DESTDIR)$(PREFIX)/lib/nelua/lib/riv_types.nelua

install-dev: install-c-dev install-nelua-dev

version:
	@echo $(RIV_VERSION_MAJMIN)
