FROM archlinux:base-devel

# Install build tools
RUN pacman -Syyu --noconfirm && \
    pacman -S --noconfirm git wget emscripten

# Install cartesi machine
RUN <<EOF
set -e
git clone --branch feature/optim-fetch --depth 1 https://github.com/cartesi/machine-emulator.git
cd machine-emulator
make bundle-boost
wget -O add-generated-files.diff https://github.com/cartesi/machine-emulator/releases/download/v0.18.0/add-generated-files.diff
patch -Np0 < add-generated-files.diff
source /etc/profile.d/emscripten.sh
make libcartesi.a CC=emcc CXX=em++ AR="emar rcs" OPTFLAGS="-O3 -g0" slirp=no release=yes -j$(nproc)
make install-headers install-static-libs STRIP=emstrip EMU_TO_LIB_A=src/libcartesi.a PREFIX=/usr/local/emscripten-wasm32
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
