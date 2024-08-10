FROM crazymax/osxcross:13.1-ubuntu AS osxcross

FROM ubuntu:latest
RUN apt-get update && \
    apt-get install -y clang lld libc6-dev build-essential git wget
COPY --from=osxcross /osxcross /osxcross
ENV PATH="/osxcross/bin:$PATH"
ENV LD_LIBRARY_PATH="/osxcross/lib"

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

# Install cartesi machine
RUN <<EOF
set -e
git clone --branch feature/optim-fetch --depth 1 https://github.com/cartesi/machine-emulator.git
cd machine-emulator
make bundle-boost
wget -O add-generated-files.diff https://github.com/cartesi/machine-emulator/releases/download/v0.18.0/add-generated-files.diff
patch -Np0 < add-generated-files.diff
EOF

ARG CC_PREFIX
ENV MACOSX_DEPLOYMENT_TARGET=13.1
RUN <<EOF
set -e
cd machine-emulator
make libcartesi.a CC=${CC_PREFIX}-clang CXX=${CC_PREFIX}-clang++ AR="llvm-ar-18 rcs" slirp=no release=yes -j$(nproc)
make install-headers install-static-libs STRIP=llvm-strip-18 EMU_TO_LIB_A=src/libcartesi.a PREFIX=/osxcross/SDK/MacOSX13.1.sdk/usr
cd ..
rm -rf machine-emulator
EOF
