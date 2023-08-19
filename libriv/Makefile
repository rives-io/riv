PREFIX=/usr

# Release flags
CC=gcc
CFLAGS+=-O2 -fno-stack-protector -Wall -Werror
LDFLAGS+=-s -Wl,-O1,--sort-common,--build-id=none -z relro -z now -rdynamic
STRIPFLAGS+=--strip-unneeded --remove-section=.comment
NELUA_FLAGS+=-Pnochecks -Pnoassertloc -Pnocstaticassert --release --verbose

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

update-bindings:
	nelua-lua genbindings.lua

clean:
	rm -f libriv.so libriv.c

install: libriv.so
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	install -m755 libriv.so $(DESTDIR)$(PREFIX)/lib/libriv.so.0.1.0
	ln -sf libriv.so.0.1.0 $(DESTDIR)$(PREFIX)/lib/libriv.so.0.1
	ln -sf libriv.so.0.1 $(DESTDIR)$(PREFIX)/lib/libriv.so.0
	ln -sf libriv.so.0 $(DESTDIR)$(PREFIX)/lib/libriv.so

	mkdir -p $(DESTDIR)$(PREFIX)/include
	install -m644 riv.h $(DESTDIR)$(PREFIX)/include/riv.h
