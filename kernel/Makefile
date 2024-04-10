KERNEL_VERSION=0.19.1-riv1
LINUX_VERSION=6.5.9-ctsi-1-v$(KERNEL_VERSION)

all: linux.bin linux-headers.tar.xz

linux.bin:
	wget -O linux.bin https://github.com/rives-io/kernel/releases/download/v$(KERNEL_VERSION)/linux-$(LINUX_VERSION).bin

linux-headers.tar.xz:
	wget -O linux-headers.tar.xz https://github.com/rives-io/kernel/releases/download/v$(KERNEL_VERSION)/linux-headers-$(LINUX_VERSION).tar.xz

distclean:
	rm -f linux.bin linux-headers.tar.xz
