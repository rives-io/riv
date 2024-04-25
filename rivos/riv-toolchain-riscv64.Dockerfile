## Host toolchain stage
FROM cartesi/toolchain:0.17.0 AS host-tools-stage
RUN apt-get update && \
    apt-get install -y squashfs-tools && \
    rm -rf /var/lib/apt

################################
# Busybox stage
FROM --platform=linux/riscv64 riscv64/busybox:1.36.1-musl AS busybox-stage

################################
# Toolchain stage
FROM --platform=linux/riscv64 riscv64/alpine:20240329 AS riv-toolchain-stage

# Update and install development packages
RUN echo "@testing https://dl-cdn.alpinelinux.org/alpine/edge/testing" >> /etc/apk/repositories && \
    apk update && \
    apk upgrade && \
    apk add build-base pkgconf git

# Build other packages inside /root
WORKDIR /root

################################
# Build elfkickers
FROM --platform=linux/riscv64 riv-toolchain-stage AS elfkickers-stage
RUN wget -O BR903-ELFkickers.tar.gz https://github.com/BR903/ELFkickers/tarball/e7fba942df51e756897224cff5aa853de8fafd90 && \
    tar -xzf BR903-ELFkickers.tar.gz && \
    mv BR903-ELFkickers-* BR903-ELFkickers && cd BR903-ELFkickers && \
    make prefix=/pkg/usr && \
    mkdir -p /pkg/usr && \
    make install prefix=/pkg/usr && \
    rm -rf /pkg/usr/share && \
    strip /pkg/usr/bin/* && \
    tree /pkg/usr && cp -a /pkg/usr/* /usr/

################################
# Build mir jit
FROM --platform=linux/riscv64 riv-toolchain-stage AS mirjit-stage
RUN wget -O vnmakarov-mir.tar.gz https://github.com/vnmakarov/mir/tarball/5dcba9a5e500f821dafbbf1db729742038bc5a80 && \
    tar -xzf vnmakarov-mir.tar.gz && \
    mv vnmakarov-mir-* vnmakarov-mir && cd vnmakarov-mir && \
    echo "echo fail" > check-threads.sh && \
    mkdir -p /pkg/usr && \
    make PREFIX=/pkg/usr && \
    make install PREFIX=/pkg/usr && \
    rm -f /pkg/usr/lib/*.a && \
    strip /pkg/usr/bin/* && \
    strip -S -x /pkg/usr/lib/*.so.* && \
    tree /pkg/usr && cp -a /pkg/usr/* /usr/

################################
# Build nelua
FROM --platform=linux/riscv64 riv-toolchain-stage AS nelua-stage
RUN wget -O edubart-nelua-lang.tar.gz https://github.com/edubart/nelua-lang/tarball/9f75e009db190feda0f90ae858b48fd82f51b8b1 && \
    tar -xzf edubart-nelua-lang.tar.gz && \
    mv edubart-nelua-lang-* edubart-nelua-lang && cd edubart-nelua-lang && \
    mkdir -p /pkg/usr && \
    CFLAGS=-march=rv64g make PREFIX=/pkg/usr nelua-lua nelua-luac && \
    make install PREFIX=/pkg/usr && \
    cp nelua-luac /pkg/usr/bin/nelua-luac && \
    strip /pkg/usr/bin/nelua-lua /pkg/usr/bin/nelua-luac && \
    ln -s nelua-lua /pkg/usr/bin/lua5.4 && \
    ln -s nelua-luac /pkg/usr/bin/luac5.4 && \
    tree /pkg/usr && cp -a /pkg/usr/* /usr/

################################
# Build cffi-lua
FROM --platform=linux/riscv64 riv-toolchain-stage AS cffi-lua-stage
RUN apk add cmake meson lua5.4-dev lua5.4-libs libffi-dev
RUN mkdir -p /pkg/usr/lib /pkg/usr/include && \
    cp -L /usr/lib/lua5.4/liblua.so /pkg/usr/lib/liblua5.4.so && \
    cp -L /usr/lib/lua5.4/liblua.a /pkg/usr/lib/liblua5.4.a && \
    cp -R /usr/include/lua5.4 /pkg/usr/include/lua5.4
RUN wget -O q66-cffi-lua.tar.gz https://github.com/q66/cffi-lua/tarball/2e0c577c4c3aad3da543040200d1303798780616 && \
    tar -xzf q66-cffi-lua.tar.gz && \
    mv q66-cffi-lua-* q66-cffi-lua && cd q66-cffi-lua && \
    mkdir -p /pkg/usr && \
    meson build -Dlua_version=5.4 -Dtests=false -Dprefix=/pkg/usr && \
    ninja -C build && \
    ninja -C build install && \
    strip -S -x /pkg/usr/lib/*.so && \
    strip -S -x /pkg/usr/lib/lua/5.4/*.so && \
    tree /pkg/usr && cp -a /pkg/usr/* /usr/

################################
# Build quickjs-ffi
FROM --platform=linux/riscv64 riv-toolchain-stage AS quickjs-ffi-stage
RUN apk add libffi libffi-dev quickjs quickjs-dev
RUN wget -O edubart-quickjs-ffi.tar.gz https://github.com/edubart/quickjs-ffi/tarball/b9b019116ee24e56d8d203e58607334678689dda && \
    tar -xzf edubart-quickjs-ffi.tar.gz && \
    mv edubart-quickjs-ffi-* edubart-quickjs-ffi && cd edubart-quickjs-ffi && \
    make && \
    mkdir -p /pkg/usr/lib/qjs && \
    cp quickjs-ffi.so quickjs-ffi.js /pkg/usr/lib/qjs/ && \
    strip -S -x /pkg/usr/lib/qjs/*.so && \
    tree /pkg/usr && cp -a /pkg/usr/* /usr/

################################
# Build bubblewrap
FROM --platform=linux/riscv64 riv-toolchain-stage AS bubblewrap-stage
RUN apk add autoconf automake libcap-dev libcap-static
RUN wget -O containers-bubblewrap.tar.gz https://github.com/containers/bubblewrap/tarball/8e51677abd7e3338e4952370bf7d902e37d8cbb6 && \
    tar -xzf containers-bubblewrap.tar.gz && \
    mv containers-bubblewrap-* containers-bubblewrap && cd containers-bubblewrap && \
    ./autogen.sh && \
    ./configure --prefix=/pkg/usr --enable-require-userns LDFLAGS=-static && \
    mkdir -p /pkg/usr && \
    make PREFIX=/pkg/usr && \
    make install PREFIX=/pkg/usr && \
    rm -rf /pkg/usr/share && \
    strip /pkg/usr/bin/* && \
    tree /pkg/usr && cp -a /pkg/usr/* /usr/

################################
# Build bwrapbox
FROM --platform=linux/riscv64 riv-toolchain-stage AS bwrapbox-stage
RUN apk add libseccomp-dev
RUN wget -O edubart-bwrapbox.tar.gz https://github.com/edubart/bwrapbox/tarball/236cca9a29b551335444a1e902012e8b0e55293f && \
    tar -xzf edubart-bwrapbox.tar.gz && \
    mv edubart-bwrapbox-* edubart-bwrapbox && cd edubart-bwrapbox && \
    mkdir -p /pkg/usr && \
    make PREFIX=/pkg/usr && \
    make install PREFIX=/pkg/usr && \
    strip /pkg/usr/bin/* && \
    tree /pkg/usr && cp -a /pkg/usr/* /usr/

################################
# Build xhalt
FROM --platform=linux/riscv64 riv-toolchain-stage AS xhalt-stage
RUN apk add libseccomp-dev
RUN wget -O xhalt.c https://raw.githubusercontent.com/cartesi/machine-emulator-tools/158948a343e792c181a8cee6964cea122c644c52/sys-utils/xhalt/xhalt.c && \
    mkdir -p /pkg/usr/sbin/ && \
    gcc xhalt.c -Os -s -o /pkg/usr/sbin/xhalt && \
    strip /pkg/usr/sbin/xhalt

################################
# Build riv
FROM --platform=linux/riscv64 nelua-stage AS libriv-stage
COPY libs/guest-host riv/libs/guest-host
COPY libriv riv/libriv
RUN make -C riv/libriv

################################
# Download packages
FROM --platform=linux/riscv64 riv-toolchain-stage AS rivos-sdk-stage

# Install development utilities
RUN apk add bash \
        neovim neovim-doc \
        htop tmux ncdu \
        gdb strace \
        squashfs-tools e2fsprogs e2fsprogs-extra \
        curl wget \
        jq \
        libffi libffi-dev \
        quickjs quickjs-dev \
        micropython@testing

# Make vim an alias to nvim
RUN ln -s /usr/bin/nvim /usr/bin/vim

# Download apks to be installed in rivos
RUN mkdir -p apks && \
    cd apks && \
    apk fetch musl libgcc libstdc++ libffi

# Install linux-headers
WORKDIR /root
COPY kernel/linux-headers.tar.xz linux-headers.tar.xz
RUN tar -xf linux-headers.tar.xz && \
    cp -R include/* /usr/include/ && \
    rm -rf include && \
    rm -f linux-headers.tar.xz

# Overwrite busybox
COPY --from=busybox-stage /bin/busybox /bin/busybox
COPY --from=elfkickers-stage /pkg/usr /usr
COPY --from=mirjit-stage /pkg/usr /usr
COPY --from=nelua-stage /pkg/usr /usr
COPY --from=bwrapbox-stage /pkg/usr /usr
COPY --from=bubblewrap-stage /pkg/usr /usr
COPY --from=cffi-lua-stage /pkg/usr /usr
COPY --from=quickjs-ffi-stage /pkg/usr /usr
COPY --from=xhalt-stage /pkg/usr /usr

# Install skel
COPY rivos/skel/etc /etc
COPY rivos/skel/usr /usr
RUN mkdir -p /cartridge /cartridges /workspace && \
    chown 500:500 /cartridge && \
    chown 1000:1000 /workspace && \
    chown root:root /root && \
    rm -rf /media

################################
# Rootfs stage

WORKDIR /rivos

# Create base filesystem hierarchy
RUN mkdir -p usr/bin usr/sbin usr/lib var/tmp proc sys dev root cartridge cartridges tmp run etc bin sbin lib workspace && \
    chmod 555 proc sys && \
    chown 500:500 cartridge && \
    chmod 1777 tmp var/tmp && \
    ln -s /run var/run && \
    ln -s ../proc/mounts etc/mtab && \
    ln -s ../../lib/ld-musl-riscv64.so.1 usr/lib/libc.so

# Install apks
RUN cp -a /etc/apk etc/apk && \
    apk add --no-network --root /rivos --initdb /root/apks/*.apk && \
    apk info -L musl-dev | grep usr/include | while read file; do install -Dm644 /$file $file; done && \
    rm -rf /root/apks /var/cache/apk etc/apk lib/apk var/cache dev/* etc/apk etc/apk/world

# Install busybox
RUN cp /bin/busybox bin/busybox && \
    for i in $(bin/busybox --list-long | grep -v sbin/init | grep -v linuxrc); do \
        ln -s /bin/busybox "/rivos/$i"; \
    done && \
    rm -rf /rivos/linuxrc /linuxrc

# Install bubblewrap and bwrapbox
RUN cp -a /usr/bin/bwrap usr/bin/bwrap && \
    cp -a /usr/bin/bwrapbox usr/bin/bwrapbox && \
    cp -a /usr/lib/bwrapbox usr/lib/bwrapbox

# Install c2m
RUN cp -a /usr/bin/c2m usr/bin/c2m && \
    cp -a /usr/include/c2mir.h usr/include/ && \
    cp -a /usr/include/mir* usr/include/

# Install cffi
RUN cp -aL /usr/lib/libffi.so* usr/lib/

# Install lua + cffi-lua
RUN cp -aL /usr/bin/lua5.4 usr/bin/lua5.4 && \
    cp -aL /usr/lib/liblua5.4.so usr/lib/liblua5.4.so && \
    cp -aL /usr/include/lua5.4 usr/include/lua5.4 && \
    cp -a /usr/lib/lua usr/lib/lua

# Install quickjs + quickjs-ffi
RUN cp -aL /usr/bin/qjs usr/bin/ && \
    cp -aL /usr/lib/qjs usr/lib/qjs

# Install micropython
RUN cp -aL /usr/bin/micropython usr/bin/

# Install xhalt
RUN cp -aL /usr/sbin/xhalt usr/sbin/

# Install skel files
COPY rivos/skel/etc etc
COPY rivos/skel/usr usr
COPY rivos/skel-sdk /
RUN chmod 600 etc/shadow

################################
# Install libriv
COPY --from=libriv-stage /root/riv/libriv /root/riv/libriv
RUN make -C /root/riv/libriv install install-dev PREFIX=/usr && \
    make -C /root/riv/libriv install install-c-dev PREFIX=/usr DESTDIR=/rivos && \
    rm -rf /root/riv

################################
# Generate rivos.ext2
FROM host-tools-stage AS generate-rivos-stage
COPY --from=rivos-sdk-stage / /rivos-sdk
RUN xgenext2fs \
    --faketime \
    --allow-holes \
    --block-size 4096 \
    --bytes-per-inode 4096 \
    --volume-label rivos --root /rivos-sdk/rivos /rivos.ext2 && \
    xgenext2fs \
        --faketime \
        --allow-holes \
        --readjustment +$((64*1024*1024/4096)) \
        --block-size 4096 \
        --bytes-per-inode 4096 \
        --volume-label rivos-sdk --root /rivos-sdk /rivos-sdk.ext2

################################
FROM --platform=linux/riscv64 rivos-sdk-stage AS toolchain-rivos-final-stage
COPY --from=generate-rivos-stage /rivos.ext2 /rivos.ext2
COPY --from=generate-rivos-stage /rivos-sdk.ext2 /rivos-sdk.ext2

# Install workaround to run env as current user
COPY --chmod=755 rivos/docker-entrypoint.sh /usr/sbin/docker-entrypoint.sh
ENTRYPOINT ["/usr/sbin/docker-entrypoint.sh"]
CMD ["/bin/bash", "-l"]
