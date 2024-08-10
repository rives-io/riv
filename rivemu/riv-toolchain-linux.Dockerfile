FROM gcc:13

RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y git wget meson glib2.0-dev python3 libgl-dev xorg-dev libboost1.81-dev glib2.0-dev

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
wget -O add-generated-files.diff https://github.com/cartesi/machine-emulator/releases/download/v0.18.0/add-generated-files.diff
patch -Np0 < add-generated-files.diff
make -C src libcartesi.a slirp=yes release=yes -j$(nproc)
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

# Install arc4random so we can avoid arc4random@GLIBC_2.36
RUN <<EOF
git clone --depth 1 https://github.com/opencoff/portable-lib.git
gcc -c -O2 -I./portable-lib/inc -o /usr/lib/arc4random.o portable-lib/src/arc4random.c
rm -rf portable-lib.git
EOF
