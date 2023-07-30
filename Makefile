all: rootfs rivcm

rootfs rivcm:
	$(MAKE) -C $@

libriv bwrapbox:
	$(MAKE) toolchain-exec COMMAND="make -C $@"

rootfs-env toolchain-env toolchain-exec:
	$(MAKE) -C rootfs $@

clean:
	$(MAKE) -C bwrapbox clean
	$(MAKE) -C rootfs clean
	$(MAKE) -C libriv clean
	$(MAKE) -C rivcm clean
	$(MAKE) -C demos clean

.PHONY: rootfs libriv bwrapbox
