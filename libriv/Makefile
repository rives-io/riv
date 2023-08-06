CC=gcc
CFLAGS=-O2 -fno-stack-protector -Wall -Werror
LDFLAGS=-s -Wl,-O1,--sort-common,--build-id=none -z relro -z now -rdynamic
STRIPFLAGS=--strip-unneeded --remove-section=.comment
NELUA_FLAGS=-Pnogc -Pnochecks -Pnoassertloc -Pnocstaticassert --release --verbose
PREFIX=/usr/local

libriv.so: libriv.nelua *.nelua
	nelua $(NELUA_FLAGS) \
		--cc=$(CC) \
		--cflags="$(CFLAGS)" \
		--shared-lib \
		--ldflags="$(LDFLAGS)" \
		--strip-bin \
		--stripflags="$(STRIPFLAGS)" \
		--cache-dir . \
		--output $@ $<

clean:
	rm -f libriv.so libriv.c

install: libriv.so
	install -m755 libriv.so $(DESTDIR)$(PREFIX)/lib/libriv.so
	install -m644 riv.h $(DESTDIR)$(PREFIX)/lib/riv.h
