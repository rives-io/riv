PREFIX=/usr/local

# Release flags
CC=gcc
CFLAGS+=-Wall
LDFLAGS+=-Wl,-O1,--sort-common,--build-id=none -z relro -z now
NELUA_FLAGS+=-Pnochecks -Pnoassertloc -Pnocstaticassert --release --verbose

rivcm: rivcm.nelua *.nelua
	nelua $(NELUA_FLAGS) \
		--cc=$(CC) \
		--cflags="$(CFLAGS) $(INCS)" \
		--ldflags="$(LDFLAGS) $(LIBS)" \
		--cache-dir . \
		--output $@ $<

clean:
	rm -f rivcm rivcm.c

install: rivcm
	mkdir -p $(DESTDIR)$(PREFIX)/rivcm
	install -m755 rivcm $(DESTDIR)$(PREFIX)/bin/rivcm
