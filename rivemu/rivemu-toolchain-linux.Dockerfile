FROM ubuntu:22.04

# Install build tools
RUN <<EOF
set -e
apt-get update
apt-get upgrade -y
apt-get install -y build-essential git wget xorg-dev libgl-dev lua5.4-dev
EOF

# Install cartesi machine
RUN <<EOF
set -e
git clone --branch v0.16.1 --depth 1 https://github.com/cartesi/machine-emulator.git
cd machine-emulator
make dep
make bundle-boost
wget -O uarch/uarch-pristine-hash.c https://github.com/cartesi/machine-emulator/releases/download/v0.16.1/uarch-pristine-hash.c
wget -O uarch/uarch-pristine-ram.c https://github.com/cartesi/machine-emulator/releases/download/v0.16.1/uarch-pristine-ram.c
make -C src relwithdebinfo=yes libcartesi.a -j$(nproc)
make install-static-libs install-headers PREFIX=/usr/local EMU_TO_LIB_A=src/libcartesi.a
cd ..
rm -rf machine-emulator
EOF

# Install nelua
RUN <<EOF
git clone --depth 1 https://github.com/edubart/nelua-lang.git
cd nelua-lang
make
make install PREFIX=/usr/local
cd ..
rm -rf nelua-lang
EOF
