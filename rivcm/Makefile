CC=gcc
CFLAGS=-Wall -Werror
LDFLAGS=-Wl,-O1,--sort-common,--build-id=none -z relro -z now
NELUA_FLAGS=-Pnogc --verbose
PREFIX=/usr/local
NELUA_LIBS=-L../libriv -L../deps/host -L../deps/guest-host

# for release
# NELUA_FLAGS+= -Pnochecks -Pnoassertloc -Pnocstaticassert --release

rivcm: rivcm.nelua *.nelua
	nelua $(NELUA_FLAGS) $(NELUA_LIBS) \
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
