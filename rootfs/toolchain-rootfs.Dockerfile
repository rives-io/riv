################################
# Busybox stage
FROM --platform=linux/riscv64 riscv64/busybox:1.36 AS busybox-stage

################################
# Toolchain stage
FROM --platform=linux/riscv64 riscv64/alpine:20231219 AS toolchain-rootfs-stage

RUN echo "@testing https://dl-cdn.alpinelinux.org/alpine/edge/testing" >> /etc/apk/repositories

RUN apk update && apk upgrade

# Add development packages
RUN apk add gcc clang g++ libc-dev make cmake pkgconfig git automake autoconf

# Install utilities
RUN apk add bash squashfs-tools su-exec bubblewrap

# Install development dependencies
RUN apk add libarchive-dev libseccomp-dev

# Install tcc
RUN apk add tcc@testing tcc-libs@testing tcc-dev@testing

# Build other packages inside /root
WORKDIR /root

# Install elfkickers
RUN wget -O BR903-ELFkickers.tar.gz https://github.com/BR903/ELFkickers/tarball/e7fba942df51e756897224cff5aa853de8fafd90 && \
    tar -xzf BR903-ELFkickers.tar.gz && \
    cd BR903-ELFkickers-*  && \
    make && \
    make install prefix=/usr && \
    cd .. && \
    rm -rf BR903-ELFkickers-*

# Install genext2fs
RUN wget -O genext2fs.tar.gz https://github.com/cartesi/genext2fs/archive/refs/tags/v1.5.3.tar.gz && \
    tar -xzf genext2fs.tar.gz && \
    cd genext2fs-* && \
    ./autogen.sh && \
    ./configure && \
    make PREFIX=/usr && \
    make install PREFIX=/usr && \
    cd .. && \
    rm -rf genext2fs-*

# Install nelua
RUN wget -O nelua-lang.tar.gz https://github.com/edubart/nelua-lang/archive/refs/tags/20240113.tar.gz && \
    tar -xzf nelua-lang.tar.gz && \
    cd nelua-lang-* && \
    make && \
    make install PREFIX=/usr && \
    cd .. && \
    rm -rf nelua-lang.tar.gz nelua-lang-*

# Install bwrapbox
RUN wget -O bwrapbox.tar.gz https://github.com/edubart/bwrapbox/archive/refs/tags/v0.2.2.tar.gz && \
    tar -xzf bwrapbox.tar.gz && \
    cd bwrapbox-* && \
    make && \
    make install PREFIX=/usr && \
    cd .. && \
    rm -rf bwrapbox-*

# Install rndaddentropy
RUN wget -O rfinnie-twuewand.tar.gz https://github.com/rfinnie/twuewand/tarball/d73ba08d81f80c04f87bb1f97ee7216ca16f202d && \
    tar -xzf rfinnie-twuewand.tar.gz && \
    cd rfinnie-twuewand-* && \
    sed -i 's|sys/fcntl.h|fcntl.h|g' rndaddentropy/rndaddentropy.c && \
    export LDFLAGS="-Wl,-O1,--sort-common,--build-id=none,--hash-style=gnu,--relax,--sort-common,--sort-section=name -z relro -z now -s" && \
    make -C rndaddentropy && \
    make -C rndaddentropy install PREFIX=/usr && \
    cd .. && \
    rm -rf rfinnie-twuewand-*

# Download apks to be installed in rootfs
WORKDIR /root/apks
RUN apk fetch \
        musl libgcc libstdc++ bubblewrap libcap2
RUN apk fetch pkgconfig tcc tcc-libs tcc-dev tcc-libs-static

# Install linux and linux-headers
WORKDIR /root
COPY kernel/linux.bin linux.bin
COPY kernel/linux-headers.tar.xz linux-headers.tar.xz
RUN apk add linux-headers && \
    cd / && rm `apk info -L linux-headers | grep -F ".h"`
RUN tar -xf linux-headers.tar.xz && \
    cp -R opt/riscv/kernel/work/linux-headers/include/* /usr/include/ && \
    rm -rf opt linux-headers*

# Install workaround to run env as current user
RUN adduser -D -u 500 cartridge cartridge
COPY --chmod=755 rootfs/toolchain-entrypoint.sh /usr/local/bin/toolchain-entrypoint.sh
ENTRYPOINT ["/usr/local/bin/toolchain-entrypoint.sh"]
CMD ["/bin/bash", "-l"]

################################
# Rootfs stage

# Create Linux filesystem hierarchy
WORKDIR /rootfs
RUN mkdir -p usr/bin usr/sbin usr/lib var/tmp proc sys dev root cartridge tmp run etc && \
    chmod 555 proc sys && \
    chown 500:500 cartridge && \
    chmod 1777 tmp var/tmp && \
    ln -s usr/bin bin && \
    ln -s usr/sbin sbin && \
    ln -s usr/lib lib && \
    ln -s /run var/run

# Install busybox
COPY --from=busybox-stage /bin/busybox bin/busybox
RUN for i in $(bin/busybox --list-long | grep -v sbin/init | grep -v linuxrc); do ln -s /bin/busybox "/rootfs/$i"; done

# Install apks
RUN cp -a /etc/apk etc/apk && \
    rm etc/apk/world && \
    apk add --no-network --root /rootfs --initdb /root/apks/*.apk && \
    rm -rf etc/apk lib/apk var/cache dev/*

# Install musl utilities
RUN ln -s ld-musl-riscv64.so.1 lib/ld-musl.so && \
    ln -s ld-musl-riscv64.so.1 lib/ld-linux-riscv64-lp64d.so.1 && \
    ln -s ../../lib/ld-musl-riscv64.so.1 usr/lib/libc.so && \
    cp -a /usr/bin/ldd usr/bin/ldd && \
    apk info -L musl-dev | grep usr/include | while read file; do install -Dm644 /$file $file; done && \
    apk info -L musl-dev | grep usr/lib | grep crt | while read file; do install -Dm644 /$file $file; done

# Install bwrapbox and rndaddentropy
RUN cp -a /usr/bin/bwrapbox /rootfs/usr/bin/bwrapbox && \
    cp -a /usr/lib/bwrapbox /rootfs/usr/lib/bwrapbox && \
    cp -a /usr/sbin/rndaddentropy /rootfs/usr/sbin/rndaddentropy

# Install system configs
COPY rootfs/skel/etc etc
RUN ln -s ../proc/mounts etc/mtab && \
    chmod 600 etc/shadow && \
    sed -i '/^ *# /d' etc/sysctl.conf

# Install init
COPY rootfs/skel/sbin sbin
RUN sed -i '/^ *# /d;/^$/d' sbin/init && \
    mkdir -p opt/cartesi/bin && \
    ln -s ../../../sbin/init opt/cartesi/bin/init

################################
# Build and install riv stuff
WORKDIR /root

# Copy libs
COPY libs/guest-host libs/guest-host

# Install libriv
COPY libriv libriv
RUN make -C libriv install install-dev PREFIX=/usr && \
    make -C libriv install install-c-dev PREFIX=/usr DESTDIR=/rootfs

# Fix crt1.o for linking with TCC
RUN gcc -Os -ffreestanding -fPIC -c libriv/crt1.c -o /rootfs/usr/lib/crt1.o

# Install riv tools
COPY tools tools
RUN make -C tools install PREFIX=/usr && \
    make -C tools install PREFIX=/usr DESTDIR=/rootfs

# Generate rootfs.ext2
RUN xgenext2fs --faketime --block-size 4096 --readjustment +0 --root /rootfs /root/rootfs.ext2
