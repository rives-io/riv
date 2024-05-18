PREFIX=/usr/local

# Release flags
CC=gcc
CFLAGS+=-Wall
NELUA_FLAGS+=-Pnochecks -Pnoerrorloc -Pnocstaticassert --verbose
NELUA_CACHEDIR=.
ifeq ($(CC), tcc)
	CFLAGS+=-w
else ifeq ($(CC), emcc)
	# WebAssembly
	RIVEMU=rivemu.js
	CFLAGS+=-sNO_DISABLE_EXCEPTION_CATCHING
	CFLAGS+=-I/usr/local/emscripten-wasm32/include
	LDFLAGS+=-L/usr/local/emscripten-wasm32/lib
else ifeq ($(CC), x86_64-w64-mingw32-gcc)
	# Windows
	RIVEMU=rivemu.exe
	CFLAGS+=-Wno-unknown-pragmas
	LDFLAGS+=-Wl,--gc-sections
	LDFLAGS+=-Wl,-O1,--sort-common,--build-id=none,--as-needed -static
else ifeq ($(CC), gcc)
	# Linux
	RIVEMU=rivemu
	LDFLAGS+=-Wl,--gc-sections
	LDFLAGS+=-Wl,-O1,--sort-common,--build-id=none,--as-needed -z relro -z now
else
	# MacOs or some other POSIX
	RIVEMU=rivemu
endif

ifeq ($(debug), yes)
	CFLAGS+=-Og -g
else # release
	ifeq ($(CC), emcc)
		# wasm
		CFLAGS+=-O3 -DNDEBUG -g0 -s
	else ifeq ($(CC), gcc)
		# linux
		CFLAGS+=-O2 -DNDEBUG -g0 -s
	else # posix
		CFLAGS+=-O2 -DNDEBUG -g0
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

all: $(RIVEMU)

$(RIVEMU): rivemu.nelua $(DEPS)
	nelua $(NELUA_FLAGS) $(EXTRA_NELUA_FLAGS) \
		--cc=$(CC) \
		--cflags="$(CFLAGS) $(INCS) $(EXTRA_CFLAGS)" \
		--ldflags="$(LDFLAGS) $(LIBS) $(EXTRA_LDFLAGS)" \
		--cache-dir="$(NELUA_CACHEDIR)" \
		--output $@ $<
	touch $@

clean:
	rm -f rivemu rivemu.c rivemu.js rivemu.wasm rivemu-*-*

distclean: clean
	docker rmi \
		riv/toolchain-wasm \
		riv/toolchain-linux-amd64 \
		riv/toolchain-linux-arm64 \
		riv/toolchain-macos-amd64 \
		riv/toolchain-macos-arm64 \
		riv/toolchain-windows-amd64 \
		|| true

install: rivemu
	mkdir -p $(DESTDIR)$(PREFIX)/rivemu
	install -m755 rivemu $(DESTDIR)$(PREFIX)/bin/rivemu

# Packages
dist: rivemu.wasm \
	rivemu-linux-amd64 rivemu-linux-arm64 \
	rivemu-macos-amd64 rivemu-macos-arm64 \
	rivemu-windows-amd64.exe

# Packaging for Linux
LINUX_WRAPPER_LDFLAGS=\
	/usr/include/X11GL-wrappers/libGL.so.init.c \
	/usr/include/X11GL-wrappers/libGL.so.tramp.S \
	/usr/include/X11GL-wrappers/libX11.so.init.c \
	/usr/include/X11GL-wrappers/libX11.so.tramp.S \
	/usr/include/X11GL-wrappers/libXcursor.so.init.c \
	/usr/include/X11GL-wrappers/libXcursor.so.tramp.S \
	/usr/include/X11GL-wrappers/libXi.so.init.c \
	/usr/include/X11GL-wrappers/libXi.so.tramp.S \
	/usr/lib/arc4random.o \
	-static-libgcc
rivemu-linux-%: rivemu.nelua $(DEPS) riv-toolchain-linux.Dockerfile
	docker build --progress plain \
		--platform linux/$* \
		--tag riv/toolchain-linux-$* \
		--file riv-toolchain-linux.Dockerfile .
	docker run \
		--platform linux/$* \
		--volume "$(shell pwd)/..":/mnt \
		--workdir /mnt \
		--hostname riv-toolchain-linux-$* \
		--rm riv/toolchain-linux-$* \
		bash -c "make -C /mnt/rivemu rivemu-linux-$* \
					RIVEMU=rivemu-linux-$* \
					EXTRA_LDFLAGS='$(LINUX_WRAPPER_LDFLAGS)' \
					EXTRA_NELUA_FLAGS=-DCARTESI_STATIC \
					NELUA_CACHEDIR=/tmp/nelua && \
				chown $(shell id -u):$(shell id -g) /mnt/rivemu/rivemu-linux-$*"

# Packaging for MacOS
rivemu-macos-amd64: CC_PREFIX=o64
rivemu-macos-arm64: CC_PREFIX=oa64
rivemu-macos-%: rivemu.nelua $(DEPS) riv-toolchain-macos.Dockerfile
	docker build --progress plain \
		--build-arg CC_PREFIX=$(CC_PREFIX) \
		--tag riv/toolchain-macos-$* \
		--file riv-toolchain-macos.Dockerfile .
	docker run \
		--volume "$(shell pwd)/..":/mnt \
		--workdir /mnt \
		--hostname riv-toolchain-macos-$* \
		--rm riv/toolchain-macos-$* \
		bash -c "make -C /mnt/rivemu rivemu-macos-$* \
					RIVEMU=rivemu-macos-$* \
					CC=$(CC_PREFIX)-clang \
					EXTRA_NELUA_FLAGS=-DCARTESI_STATIC \
					NELUA_CACHEDIR=/tmp/nelua && \
				chown $(shell id -u):$(shell id -g) /mnt/rivemu/rivemu-macos-$*"

# Packaging for Windows
rivemu-windows-amd64: rivemu-windows-amd64.exe
rivemu-windows-%.exe: rivemu.nelua $(DEPS) riv-toolchain-windows.Dockerfile
	docker build --progress plain \
		--tag riv/toolchain-windows-amd64 \
		--file riv-toolchain-windows.Dockerfile .
	docker run \
		--volume "$(shell pwd)/..":/mnt \
		--workdir /mnt \
		--hostname riv-toolchain-windows-amd64 \
		--rm riv/toolchain-windows-amd64 \
		bash -c "make -C /mnt/rivemu rivemu-windows-amd64.exe \
					RIVEMU=rivemu-windows-amd64.exe \
					CC=x86_64-w64-mingw32-gcc \
					EXTRA_NELUA_FLAGS=-DCARTESI_STATIC \
					NELUA_CACHEDIR=/tmp/nelua && \
				chown $(shell id -u):$(shell id -g) /mnt/rivemu/rivemu-windows-amd64.exe"

# Packaging for WASM
rivemu.wasm: rivemu.nelua $(DEPS) riv-toolchain-wasm.Dockerfile
	docker build --progress plain \
		--tag riv/toolchain-wasm \
		--file riv-toolchain-wasm.Dockerfile .
	docker run \
		--volume "$(shell pwd)/..":/mnt \
		--workdir /mnt \
		--hostname riv-toolchain-wasm \
		--rm riv/toolchain-wasm \
		bash -c "source /etc/profile.d/emscripten.sh && \
				make -C /mnt/rivemu \
					RIVEMU=rivemu.js \
					CC=emcc \
					EXTRA_NELUA_FLAGS=-DCARTESI_STATIC \
					NELUA_CACHEDIR=/tmp/nelua && \
				chown $(shell id -u):$(shell id -g) /mnt/rivemu/rivemu.js /mnt/rivemu/rivemu.wasm"

# External deps
../kernel/linux.bin.zz:
	$(MAKE) -C ../kernel

../rivos/rivos.ext2.zz:
	$(MAKE) -C ../rivos
