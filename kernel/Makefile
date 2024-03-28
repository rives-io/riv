KERNEL_VERSION=0.19.1
LINUX_VERSION=6.5.9-ctsi-1

all: linux.bin linux-headers.tar.xz

linux.bin:
	wget -O linux.bin https://github.com/cartesi/image-kernel/releases/download/v$(KERNEL_VERSION)/linux-$(LINUX_VERSION)-v$(KERNEL_VERSION).bin

linux-headers.tar.xz:
	wget -O linux-headers.tar.xz https://github.com/cartesi/image-kernel/releases/download/v$(KERNEL_VERSION)/linux-headers-$(LINUX_VERSION)-v$(KERNEL_VERSION).tar.xz

distclean:
	rm -f linux.bin
