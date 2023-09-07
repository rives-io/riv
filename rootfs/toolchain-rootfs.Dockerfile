################################
# Busybox stage
FROM --platform=linux/riscv64 busybox:latest AS busybox-stage

################################
# Toolchain stage
FROM --platform=linux/riscv64 alpine:edge AS toolchain-rootfs-stage

RUN apk update && apk upgrade

# Add development packages
RUN apk add gcc g++ git libc-dev make cmake pkgconfig automake autoconf

# Install utilities
RUN apk add bash squashfs-tools su-exec bubblewrap

# Install other languages
RUN apk add clang go rust nim

# Install development dependencies
RUN apk add libarchive-dev libseccomp-dev

# Musl link aliases
RUN ln -s ld-musl-riscv64.so.1 /lib/ld-musl.so

# Build other packages inside /root
WORKDIR /root

# Download apks to be installed in rootfs
RUN apk fetch musl bubblewrap libcap2

# Install elfkickers
RUN wget -O BR903-ELFkickers.tar.gz https://github.com/BR903/ELFkickers/tarball/master && \
    tar -xzf BR903-ELFkickers.tar.gz && \
    cd BR903-ELFkickers-*  && \
    make -j4 && \
    make install prefix=/usr && \
    cd .. && \
    rm -rf BR903-ELFkickers-*

# Install genext2fs
RUN wget -O genext2fs-1.5.2.tar.gz https://github.com/cartesi/genext2fs/archive/refs/tags/v1.5.2.tar.gz && \
    tar -xzf genext2fs-1.5.2.tar.gz && \
    cd genext2fs-* && \
    ./autogen.sh && \
    ./configure && \
    make PREFIX=/usr && \
    make install PREFIX=/usr && \
    cd .. && \
    rm -rf genext2fs-*

# Install nelua
RUN wget -O nelua-lang-latest.tar.gz https://github.com/edubart/nelua-lang/tarball/master && \
    tar -xzf nelua-lang-latest.tar.gz && \
    cd edubart-nelua-lang-* && \
    make && \
    make install PREFIX=/usr && \
    cd .. && \
    rm -rf nelua-lang-latest.tar.gz edubart-nelua-lang-*

# Install linux-headers
COPY kernel/linux-headers.tar.xz linux-headers.tar.xz
RUN apk add linux-headers && \
    cd / && rm `apk info -L linux-headers | grep -F ".h"`
RUN tar -xf linux-headers.tar.xz && \
    cp -R opt/riscv/kernel/work/linux-headers/include/* /usr/include/ && \
    rm -rf opt linux-headers*

# Copy libs
COPY libs/guest-host libs/guest-host

# Install libriv
COPY libriv libriv
RUN make -C libriv install install-dev PREFIX=/usr

# Install riv tools
COPY tools tools
RUN make -C tools install PREFIX=/usr

################################
# Build rootfs

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
    apk add --no-network --root /rootfs --initdb /root/*.apk && \
    rm -rf etc/apk lib/apk var/cache dev/*

# Install musl utilities
RUN ln -s ld-musl-riscv64.so.1 lib/ld-musl.so && \
    cp -a /usr/bin/ldd usr/bin/ldd

# Install libriv
RUN make -C /root/libriv install PREFIX=/usr DESTDIR=/rootfs

# Install tools
RUN make -C /root/tools install PREFIX=/usr DESTDIR=/rootfs

# Install system configs
COPY rootfs/skel/etc etc
COPY rootfs/skel/usr usr
RUN ln -s ../proc/mounts etc/mtab && \
    chmod 600 etc/shadow && \
    sed -i '/^ *# /d' etc/sysctl.conf

# Install init
COPY rootfs/skel/sbin sbin
RUN sed -i '/^ *# /d;/^$/d' sbin/init
RUN mkdir -p opt/cartesi/bin && \
    ln -s ../../../sbin/init opt/cartesi/bin/init

# Generate rootfs
RUN genext2fs \
    --faketime \
    --block-size 4096 \
    --readjustment +0 \
    --root /rootfs /rootfs.ext2

# Install workaround to run env as current user
RUN adduser -D -u 500 cartridge cartridge
COPY --chmod=755 rootfs/toolchain-entrypoint.sh /usr/local/bin/toolchain-entrypoint.sh
ENTRYPOINT ["/usr/local/bin/toolchain-entrypoint.sh"]

CMD ["/bin/bash", "-l"]
