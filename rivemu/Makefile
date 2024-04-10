PREFIX=/usr/local

# Release flags
CC=gcc
CFLAGS+=-Wall
NELUA_FLAGS+=-Pnochecks -Pnoerrorloc -Pnocstaticassert --verbose
NELUA_CACHEDIR=.
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

ifeq ($(debug), yes)
	CFLAGS+=-Og -g
else # release
	ifeq ($(CC), emcc)
		CFLAGS+=-O3 -DNDEBUG -g0 -s
	else # gcc based
		CFLAGS+=-O2 -DNDEBUG -g0 -s
	endif
endif

DEPS=*.nelua \
	$(wildcard ../libs/guest-host/*.{h,c,nelua}) \
	$(wildcard ../libs/host/*.{h,c,nelua}) \
	../libriv/riv_types.nelua \
	../libriv/riv_log.nelua \
	../libriv/riv.h

$(RIVEMU): rivemu.nelua $(DEPS)
	nelua $(NELUA_FLAGS) $(NELUA_MYFLAGS) \
		--cc=$(CC) \
		--cflags="$(CFLAGS) $(INCS)" \
		--ldflags="$(LDFLAGS) $(LIBS)" \
		--cache-dir $(NELUA_CACHEDIR) \
		--output $@ $<
	touch $@

clean:
	rm -f rivemu rivemu.c rivemu.js rivemu.exe rivemu.wasm rivemu-linux-*

install: rivemu
	mkdir -p $(DESTDIR)$(PREFIX)/rivemu
	install -m755 rivemu $(DESTDIR)$(PREFIX)/bin/rivemu

build: $(RIVEMU)

distro: rivemu-linux-amd64 rivemu-linux-arm64

rivemu-linux-%:
	docker build \
		--platform linux/$* \
		--tag rivemu-toolchain-linux-$* \
		--file rivemu-toolchain-linux.Dockerfile .
	docker run \
		--platform linux/$* \
		--volume "$(shell pwd)/..":/mnt \
		--workdir /mnt \
		--hostname rivemu-build-linux-$* \
		--rm -it rivemu-toolchain-linux-$* \
		bash -c "make -C /mnt/rivemu RIVEMU=rivemu-linux-$* NELUA_CACHEDIR=/tmp/nelua rivemu-linux-$* && chown $(shell id -u):$(shell id -g) /mnt/rivemu/rivemu-linux-$*"
