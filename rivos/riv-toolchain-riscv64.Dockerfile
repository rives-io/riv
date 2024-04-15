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
RUN wget -O edubart-nelua-lang.tar.gz https://github.com/edubart/nelua-lang/tarball/05a2633a18dfdde7389394b9289da582c10e79bc && \
    tar -xzf edubart-nelua-lang.tar.gz && \
    mv edubart-nelua-lang-* edubart-nelua-lang && cd edubart-nelua-lang && \
    mkdir -p /pkg/usr && \
    make PREFIX=/pkg/usr && \
    make install PREFIX=/pkg/usr && \
    strip /pkg/usr/bin/nelua-lua && \
    ln -s nelua-lua /pkg/usr/bin/lua5.4 && \
    tree /pkg/usr && cp -a /pkg/usr/* /usr/

################################
# Build luajit
FROM --platform=linux/riscv64 riv-toolchain-stage AS luajit-stage
RUN wget -O infiWang-LJRV.tar.gz https://github.com/infiWang/LJRV/tarball/af9c41c38a285abde6d40ba575392609221bbc0f && \
    tar -xzf infiWang-LJRV.tar.gz && \
    mv infiWang-LJRV-* infiWang-LJRV && cd infiWang-LJRV && \
    mkdir -p /pkg/usr && \
    make amalg PREFIX=/usr && \
    make install PREFIX=/usr DESTDIR=/pkg && \
    rm -rf /pkg/usr/share/man /pkg/usr/bin/luajit && \
    mv /pkg/usr/bin/luajit-* /pkg/usr/bin/luajit && \
    strip /pkg/usr/bin/luajit && \
    strip -S -x /pkg/usr/lib/*.so.* && \
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
# Build riv
FROM --platform=linux/riscv64 nelua-stage AS libriv-stage
COPY libs/guest-host riv/libs/guest-host
COPY libriv riv/libriv
RUN make -C riv/libriv

################################
# Download packages
FROM --platform=linux/riscv64 riv-toolchain-stage AS rivos-sdk-stage

# Install development utilities
RUN apk add bash neovim neovim-doc htop tmux gdb strace squashfs-tools ncdu jq

# Make vim an alias to nvim
RUN ln -s /usr/bin/nvim /usr/bin/vim

# Download apks to be installed in rivos
RUN mkdir -p apks && \
    cd apks && \
    apk fetch musl libgcc libstdc++

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
COPY --from=luajit-stage /pkg/usr /usr

# Install skel
COPY rivos/skel/etc /etc
COPY rivos/skel/usr /usr
RUN ln -s ld-musl-riscv64.so.1 /lib/ld-musl.so && \
    mkdir -p /cartridge /cartridges /workspace && chown 500:500 /cartridge && \
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
    ln -s ld-musl-riscv64.so.1 lib/ld-musl.so && \
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

# Install luajit
RUN cp -a /usr/bin/luajit usr/bin/ && \
    cp -a /usr/lib/libluajit-5.1.so* usr/lib/ && \
    cp -a /usr/include/luajit-2.1 usr/include/

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
        --readjustment +$((128*1024*1024/4096)) \
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
