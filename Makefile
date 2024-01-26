all:
	$(MAKE) kernel
	$(MAKE) rootfs
	$(MAKE) rivemu
	$(MAKE) demos

all-cross:
	$(MAKE) kernel
	$(MAKE) rootfs-cross
	$(MAKE) rivemu
	$(MAKE) demos-cross

# Targets that uses the host toolchain
libs kernel rootfs rivemu rivemu-web:
	$(MAKE) -C $@

libriv-cross:
	$(MAKE) -C libriv

tools-cross:
	$(MAKE) -C tools

rootfs-cross: libriv-cross tools-cross
	$(MAKE) -C rootfs

demos-cross:
	$(MAKE) -C demos

# Targets that uses RISC-V toolchain
libriv tools demos:
	$(MAKE) -C rootfs toolchain-exec COMMAND="make -C $@"

toolchain toolchain-exec toolchain-env toolchain-env-asroot shell:
	$(MAKE) -C rootfs $@

update-libs:
	$(MAKE) -C libs update-libs

update-bindings:
	$(MAKE) -C libriv update-bindings
	$(MAKE) -C libs update-bindings

clean:
	$(MAKE) -C rootfs clean
	$(MAKE) -C libriv clean
	$(MAKE) -C rivemu clean
	$(MAKE) -C demos clean
	$(MAKE) -C tools clean
	$(MAKE) -C libs clean

distclean: clean
	$(MAKE) -C kernel distclean

download-images:
	wget -O kernel/linux.bin https://github.com/edubart/riv/releases/download/downloads/linux.bin
	wget -O kernel/rom.bin https://github.com/edubart/riv/releases/download/downloads/rom.bin
	wget -O rootfs/rootfs.ext2 https://github.com/edubart/riv/releases/download/downloads/rootfs.ext2
	wget -O kernel/linux-headers.tar.xz https://github.com/edubart/riv/releases/download/downloads/linux-headers.tar.xz

.PHONY: kernel rootfs demos rivemu libs libriv tools download-images rivemu-web

##################
# Demo testing
DEMO=snake

demo:
	$(MAKE) -C rootfs toolchain-exec COMMAND="make -C demos/$(DEMO) -j4"

demo-clean:
	$(MAKE) -C rootfs toolchain-exec COMMAND="make -C demos/$(DEMO) clean"

demo-run:
	$(MAKE) -C demos/$(DEMO) run
