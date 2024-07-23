FROM archlinux:base-devel

# Install build tools
RUN pacman -Syyu --noconfirm && \
    pacman -S --noconfirm git wget mingw-w64-toolchain

# Install cartesi machine
RUN <<EOF
set -e
git clone --branch feature/windows-virtio-9p --depth 1 https://github.com/cartesi/machine-emulator.git
cd machine-emulator
make dep
make bundle-boost
wget -O uarch/uarch-pristine-hash.c https://github.com/cartesi/machine-emulator/releases/download/v0.16.1/uarch-pristine-hash.c
wget -O uarch/uarch-pristine-ram.c https://github.com/cartesi/machine-emulator/releases/download/v0.16.1/uarch-pristine-ram.c
make libcartesi.a CC=x86_64-w64-mingw32-gcc CXX=x86_64-w64-mingw32-g++ AR="x86_64-w64-mingw32-ar rcs" TARGET_OS=Windows slirp=no release=yes -j$(nproc)
make install-headers install-static-libs STRIP=x86_64-w64-mingw32-strip EMU_TO_LIB_A=src/libcartesi.a PREFIX=/usr/x86_64-w64-mingw32
cd ..
rm -rf machine-emulator
EOF

# Install nelua
RUN <<EOF
set -e
git clone --depth 1 https://github.com/edubart/nelua-lang.git
cd nelua-lang
make
make install PREFIX=/usr/local
cd ..
rm -rf nelua-lang
EOF
