CC=gcc
CFLAGS=-O2 -fno-stack-protector -Wall -Werror
LDFLAGS=-s -Wl,-O1,--sort-common,--build-id=none -z relro -z now -rdynamic
STRIPFLAGS=--strip-unneeded --remove-section=.comment
NELUA_FLAGS=-Pnochecks -Pnoassertloc -Pnocstaticassert --release --verbose
PREFIX=/usr/local

unpack-rivlog: unpack-rivlog.nelua ../deps/guest-host/* ../libriv/*
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
