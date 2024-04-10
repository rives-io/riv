PREFIX=/usr/local

# Release flags
CC=gcc
CFLAGS+=-Wall
NELUA_FLAGS+=-Pnochecks -Pnoerrorloc -Pnocstaticassert --verbose
ifeq ($(CC), tcc)
	CFLAGS+=-w
else ifeq ($(CC), emcc)
	CFLAGS+=-sNO_DISABLE_EXCEPTION_CATCHING
else # gcc based
	LDFLAGS+=-Wl,-O1,--sort-common,--build-id=none -z relro -z now -static-libgcc
endif

ifeq ($(CC), emcc)
	RIVEMU=rivemu.js
else
	RIVEMU=rivemu
endif

ifeq ($(debug), yes) #debug
	CFLAGS+=-Og -g
else # release
	ifeq ($(CC), emcc)
		CFLAGS+=-O3 -DNDEBUG -g0 -s
	else # gcc based
		CFLAGS+=-O2 -DNDEBUG -g0
		NELUA_FLAGS+=--strip-bin
	endif
endif

DEPS=*.nelua \
	$(wildcard ../libs/guest-host/*.{h,c,nelua}) \
	$(wildcard ../libs/host/*.{h,c,nelua}) \
	../libriv/riv_types.nelua \
	../libriv/riv_log.nelua \
	../libriv/riv.h

$(RIVEMU): rivemu.nelua $(DEPS)
	nelua $(NELUA_FLAGS) \
		--cc=$(CC) \
		--cflags="$(CFLAGS) $(INCS)" \
		--ldflags="$(LDFLAGS) $(LIBS)" \
		--cache-dir . \
		--output $@ $<
	touch $@

clean:
	rm -f rivemu rivemu.c rivemu.js rivemu.exe rivemu.wasm

install: rivemu
	mkdir -p $(DESTDIR)$(PREFIX)/rivemu
	install -m755 rivemu $(DESTDIR)$(PREFIX)/bin/rivemu

build: $(RIVEMU)

build-portable-linux:
	docker build --tag riv-host-toolchain-ubuntu .