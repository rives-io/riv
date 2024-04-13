FROM ubuntu:22.04

# Install build tools
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y build-essential git wget meson glib2.0-dev python3 libgl-dev xorg-dev gcc-12 g++-12

# Build libslirp
RUN <<EOF
set -e
git clone --branch v4.7.0 --depth 1 https://gitlab.freedesktop.org/slirp/libslirp.git
cd libslirp
meson build -Ddefault_library=static
ninja -C build install
cd ..
rm -rf libslirp
EOF

# Install cartesi machine
RUN <<EOF
set -e
git clone --branch feature/optim-fetch --depth 1 https://github.com/cartesi/machine-emulator.git
cd machine-emulator
make dep
make bundle-boost
wget -O uarch/uarch-pristine-hash.c https://github.com/cartesi/machine-emulator/releases/download/v0.16.1/uarch-pristine-hash.c
wget -O uarch/uarch-pristine-ram.c https://github.com/cartesi/machine-emulator/releases/download/v0.16.1/uarch-pristine-ram.c
make -C src CC=gcc-12 CXX=g++-12 AR="gcc-ar-12 rcs" libcartesi.a slirp=yes release=yes -j$(nproc)
make install-static-libs install-headers PREFIX=/usr/local EMU_TO_LIB_A=src/libcartesi.a
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

# Install graphical library wrappers
RUN <<EOF
set -e
git clone --depth 1 https://github.com/yugr/Implib.so.git
cd Implib.so
./implib-gen.py /usr/lib/$(uname -m)-linux-gnu/libGL.so
./implib-gen.py /usr/lib/$(uname -m)-linux-gnu/libX11.so
./implib-gen.py /usr/lib/$(uname -m)-linux-gnu/libXi.so
./implib-gen.py /usr/lib/$(uname -m)-linux-gnu/libXcursor.so
mkdir -p /usr/include/X11GL-wrappers
cp *.c *.S /usr/include/X11GL-wrappers/
cd ..
rm -rf Implib.so
EOF
