PREFIX=/usr/local

# Release flags
CC=gcc
CFLAGS+=-Wall -Werror
LDFLAGS+=-s -Wl,-O1,--sort-common,--build-id=none -z relro -z now
STRIPFLAGS+=--strip-unneeded --remove-section=.comment
NELUA_FLAGS+=-Pnochecks -Pnoassertloc -Pnocstaticassert --release --verbose

all: bwrapbox seccomp-filter seccomp-filter.bpf

bwrapbox: bwrapbox.nelua
	nelua $(NELUA_FLAGS) \
		--cc=$(CC) \
		--cflags="$(CFLAGS)" \
		--ldflags="$(LDFLAGS)" \
		--strip-bin \
		--stripflags="$(STRIPFLAGS)" \
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
