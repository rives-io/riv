PREFIX=/usr/local
WASM_CC=emcc

# Release flags
CC=gcc
CFLAGS+=-Wall
LDFLAGS+=-Wl,-O1,--sort-common,--build-id=none -z relro -z now
NELUA_FLAGS+=-Pnochecks -Pnoassertloc -Pnocstaticassert --release --verbose

rivemu: rivemu.nelua *.nelua
	nelua $(NELUA_FLAGS) \
		--cc=$(CC) \
		--cflags="$(CFLAGS) $(INCS)" \
		--ldflags="$(LDFLAGS) $(LIBS)" \
		--cache-dir . \
		--output $@ $<

rivemu.js: rivemu.nelua *.nelua
	nelua $(NELUA_FLAGS) \
		--cc=$(WASM_CC) \
		--cflags="$(CFLAGS) $(INCS)" \
		--output $@ $<

clean:
	rm -rf web
	rm -f rivemu rivemu.c

install: rivemu
	mkdir -p $(DESTDIR)$(PREFIX)/rivemu
	install -m755 rivemu $(DESTDIR)$(PREFIX)/bin/rivemu
