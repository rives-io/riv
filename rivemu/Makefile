PREFIX=/usr/local
WASM_CC=emcc

# Release flags
CC=gcc
CFLAGS+=-Wall
LDFLAGS+=-Wl,-O1,--sort-common,--build-id=none -z relro -z now
NELUA_FLAGS+=-Pnochecks -Pnoerrorloc -Pnocstaticassert --verbose

ifeq ($(debug), yes)
	CFLAGS+=-Og
else
	CFLAGS+=-O2 -DNDEBUG
	NELUA_FLAGS+=--strip-bin
endif

DEPS=*.nelua \
	$(wildcard ../libs/guest-host/*.{h,c,nelua}) \
	$(wildcard ../libs/host/*.{h,c,nelua}) \
	../libriv/riv_types.nelua \
	../libriv/riv_log.nelua \
	../libriv/riv.h

rivemu: rivemu.nelua $(DEPS)
	nelua $(NELUA_FLAGS) \
		--cc=$(CC) \
		--cflags="$(CFLAGS) $(INCS)" \
		--ldflags="$(LDFLAGS) $(LIBS)" \
		--cache-dir . \
		--output $@ $<
	touch $@

rivemu.js: rivemu.nelua $(DEPS)
	nelua --release $(NELUA_FLAGS) \
		--cc=$(WASM_CC) \
		--cflags="$(CFLAGS) $(INCS)" \
		--output $@ $<
	touch $@

clean:
	rm -rf web
	rm -f rivemu rivemu.c rivemu.js rivemu.wasm

install: rivemu
	mkdir -p $(DESTDIR)$(PREFIX)/rivemu
	install -m755 rivemu $(DESTDIR)$(PREFIX)/bin/rivemu
