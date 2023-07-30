################################
# Alpine
FROM --platform=linux/riscv64 alpine:edge

RUN apk update && apk upgrade

# Add development packages
RUN apk add bash gcc g++ git libc-dev make cmake pkgconfig automake autoconf

WORKDIR /root

# Install Nelua
RUN wget -O nelua-lang-latest.tar.gz https://github.com/edubart/nelua-lang/tarball/master && \
    tar -xzf nelua-lang-latest.tar.gz && \
    cd edubart-nelua-lang-* && \
    make && \
    make install PREFIX=/usr && \
    cd .. && \
    rm -rf nelua-lang-latest.tar.gz edubart-nelua-lang-*

# Install genext2fs
RUN apk add libarchive-dev
RUN wget -O genext2fs-1.5.2.tar.gz https://github.com/cartesi/genext2fs/archive/refs/tags/v1.5.2.tar.gz && \
    tar -xzf genext2fs-1.5.2.tar.gz && \
    cd genext2fs-1.5.2 && \
    ./autogen.sh && \
    ./configure && \
    make PREFIX=/usr && \
    make install PREFIX=/usr && \
    cd .. && \
    rm -rf genext2fs*

# Install linux headers
COPY kernel/linux-headers-5.15.63-ctsi-2.tar.xz linux-headers-5.15.63-ctsi-2.tar.xz
RUN tar -xf linux-headers-5.15.63-ctsi-2.tar.xz && \
    cp -R opt/riscv/kernel/work/linux-headers/include/* /usr/include/ && \
    rm -rf opt linux-headers*

# Install other utilities
RUN apk add squashfs-tools
RUN apk add bubblewrap
RUN apk add libseccomp-dev

CMD ["/bin/bash", "-l"]
