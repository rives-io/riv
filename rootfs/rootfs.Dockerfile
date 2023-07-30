################################
# Toolchain stage
FROM --platform=linux/riscv64 riv/toolchain:devel AS toolchain-rootfs-stage

################################
## Export rootfs stage
FROM scratch AS rootfs-stage
COPY --from=toolchain-rootfs-stage /rootfs.ext2 .
