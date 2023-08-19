KERNEL_VERSION=0.17.0
ROM_VERSION=0.17.0
LINUX_VERSION=5.15.63-ctsi-2
IMAGE_KERNEL=image-kernel-$(KERNEL_VERSION)
KERNEL_TIMESTAMP=Thu, 01 Jan 1970 00:00:00 +000

.NOTPARALLEL: all
all: build copy rom.bin

build: $(IMAGE_KERNEL)/cartesi-linux-config
	$(MAKE) -C $(IMAGE_KERNEL) KERNEL_TIMESTAMP="$(KERNEL_TIMESTAMP)"

copy:
	cp -u $(IMAGE_KERNEL)/artifacts/linux-$(LINUX_VERSION)-v$(KERNEL_VERSION).bin linux.bin
	cp -u $(IMAGE_KERNEL)/artifacts/linux-headers-$(LINUX_VERSION)-v$(KERNEL_VERSION).tar.xz linux-headers.tar.xz

$(IMAGE_KERNEL)/cartesi-linux-config: config $(IMAGE_KERNEL)
	cp -u config $(IMAGE_KERNEL)/cartesi-linux-config

$(IMAGE_KERNEL): $(IMAGE_KERNEL).tar.gz
	tar -xf $(IMAGE_KERNEL).tar.gz

$(IMAGE_KERNEL).tar.gz:
	wget -O $(IMAGE_KERNEL).tar.gz https://github.com/cartesi/image-kernel/archive/refs/tags/v$(KERNEL_VERSION).tar.gz

rom.bin:
	wget -O rom.bin https://github.com/cartesi/machine-emulator-rom/releases/download/v$(ROM_VERSION)/rom-v$(ROM_VERSION).bin

# Remove all downloaded, generated and extracted files
distclean:
	rm -rf $(IMAGE_KERNEL) $(IMAGE_KERNEL).tar.gz linux.bin linux-headers.tar.xz rom.bin
