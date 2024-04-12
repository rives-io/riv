PREFIX=/usr/local

# Release flags
CC=gcc
CFLAGS+=-Wall
NELUA_FLAGS+=-Pnochecks -Pnoerrorloc -Pnocstaticassert --verbose
NELUA_CACHEDIR=.
ifeq ($(CC), tcc)
	CFLAGS+=-w
else ifeq ($(CC), emcc)
	RIVEMU=rivemu.js
	CFLAGS+=-sNO_DISABLE_EXCEPTION_CATCHING
	CFLAGS+=-I/usr/local/emscripten-wasm32/include
	LDFLAGS+=-L/usr/local/emscripten-wasm32/lib
else ifeq ($(CC), x86_64-w64-mingw32-gcc)
	RIVEMU=rivemu.exe
	CFLAGS+=-Wno-unknown-pragmas
	LDFLAGS+=-Wl,-O1,--sort-common,--build-id=none,--as-needed -static
else # probably GCC on Linux
	RIVEMU=rivemu
	LDFLAGS+=-Wl,-O1,--sort-common,--build-id=none,--as-needed -z relro -z now -static-libgcc
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
	../libriv/riv.h \
	../kernel/linux.bin.zz \
	../rivos/rivos.ext2.zz

$(RIVEMU): rivemu.nelua $(DEPS)
	nelua $(NELUA_FLAGS) $(NELUA_MYFLAGS) \
		--cc=$(CC) \
		--cflags="$(CFLAGS) $(INCS)" \
		--ldflags="$(LDFLAGS) $(LIBS)" \
		--cache-dir="$(NELUA_CACHEDIR)" \
		--output $@ $<
	touch $@

clean:
	rm -f rivemu rivemu.c rivemu.js rivemu.exe rivemu.wasm rivemu-linux-amd64 rivemu-linux-arm64 rivemu-windows-amd64.exe

distclean: clean
	docker rmi riv/toolchain-wasm riv/toolchain-linux-amd64 riv/toolchain-linux-arm64

install: rivemu
	mkdir -p $(DESTDIR)$(PREFIX)/rivemu
	install -m755 rivemu $(DESTDIR)$(PREFIX)/bin/rivemu

build: $(RIVEMU) rivemu.wasm

# Packages
package: rivemu-linux-amd64 rivemu-linux-arm64 rivemu-windows-amd64.exe rivemu.wasm

# Packaging for Linux
rivemu-linux-%: rivemu.nelua $(DEPS)
	docker build \
		--platform linux/$* \
		--tag riv/toolchain-linux-$* \
		--file riv-toolchain-linux.Dockerfile .
	docker run \
		--platform linux/$* \
		--volume "$(shell pwd)/..":/mnt \
		--workdir /mnt \
		--hostname riv-toolchain-linux-$* \
		--rm riv/toolchain-linux-$* \
		bash -c "make -C /mnt/rivemu rivemu-linux-$* RIVEMU=rivemu-linux-$* NELUA_CACHEDIR=/tmp/nelua && chown $(shell id -u):$(shell id -g) /mnt/rivemu/rivemu-linux-$*"

# Packaging for Windows
rivemu-windows-amd64: rivemu-windows-amd64.exe
rivemu-windows-%.exe: rivemu.nelua $(DEPS)
	docker build \
		--tag riv/toolchain-windows-amd64 \
		--file riv-toolchain-windows.Dockerfile .
	docker run \
		--volume "$(shell pwd)/..":/mnt \
		--workdir /mnt \
		--hostname riv-toolchain-windows-amd64 \
		--rm riv/toolchain-windows-amd64 \
		bash -c "make -C /mnt/rivemu rivemu-windows-amd64.exe RIVEMU=rivemu-windows-amd64.exe CC=x86_64-w64-mingw32-gcc NELUA_CACHEDIR=/tmp/nelua && chown $(shell id -u):$(shell id -g) /mnt/rivemu/rivemu-windows-amd64.exe"

# Packaging for WASM
rivemu.wasm: rivemu.nelua $(DEPS)
	docker build \
		--tag riv/toolchain-wasm \
		--file riv-toolchain-wasm.Dockerfile .
	docker run \
		--volume "$(shell pwd)/..":/mnt \
		--workdir /mnt \
		--hostname riv-toolchain-wasm \
		--rm riv/toolchain-wasm \
		bash -c "source /etc/profile.d/emscripten.sh && make -C /mnt/rivemu rivemu.js CC=emcc NELUA_CACHEDIR=/tmp/nelua && chown $(shell id -u):$(shell id -g) /mnt/rivemu/rivemu.js /mnt/rivemu/rivemu.wasm"

# External deps
../kernel/linux.bin.zz:
	$(MAKE) -C ../kernel

../rivos/rivos.ext2.zz:
	$(MAKE) -C ../rivos
