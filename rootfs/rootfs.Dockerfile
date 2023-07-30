################################
# Busybox
FROM --platform=linux/riscv64 busybox:latest AS busybox

################################
# Build rootfs stage
FROM --platform=linux/riscv64 riv/toolchain:devel AS rootfs-stage

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
COPY --from=busybox /bin/busybox bin/busybox
RUN for i in $(bin/busybox --list-long | grep -v sbin/init | grep -v linuxrc); do ln -s /bin/busybox "/rootfs/$i"; done

# Prepare APK
RUN cp -a /etc/apk etc/apk && \
    echo > /etc/apk/world && \
    apk add --root /rootfs --update-cache --initdb

# Install musl libc
RUN apk add --root /rootfs musl && \
    ln -s ld-musl-riscv64.so.1 lib/ld-musl.so && \
    ln -s ld-musl-riscv64.so.1 lib/ld.so && \
    cp -a /usr/bin/ldd usr/bin/ldd

# Install bubblewrap
RUN apk add --root /rootfs bubblewrap

# Remove APK
RUN rm -r lib/apk etc/apk var/cache

# Install bwrapbox
COPY bwrapbox /var/tmp/bwrapbox
RUN make -C /var/tmp/bwrapbox install PREFIX=/usr DESTDIR=/rootfs

# Install libriv
COPY libriv /var/tmp/libriv
RUN make -C /var/tmp/libriv install PREFIX=/usr DESTDIR=/rootfs

# Install system configs
COPY rootfs/skel/etc etc
COPY rootfs/skel/usr usr
RUN ln -s ../proc/mounts etc/mtab && \
    chmod 600 etc/shadow && \
    sed -i '/^ *# /d' etc/sysctl.conf

# Install init
COPY --chmod=755 rootfs/skel/sbin/init sbin/init
RUN sed -i '/^ *# /d;/^$/d' sbin/init

# Generate rootfs
RUN genext2fs \
    --faketime \
    --block-size 4096 \
    --readjustment +0 \
    --root /rootfs /rootfs.ext2

################################
FROM scratch AS export-stage
COPY --from=rootfs-stage /rootfs.ext2 .
