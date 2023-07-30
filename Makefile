rootfs toolchain:
	$(MAKE) -C $@

toolchain-env toolchain-exec:
	$(MAKE) -C toolchain $@

rootfs-env:
	$(MAKE) -C rootfs $@

libriv bwrapbox:
	$(MAKE) toolchain-exec COMMAND="make -C $@"

clean:
	$(MAKE) -C bwrapbox clean
	$(MAKE) -C rootfs clean
	$(MAKE) -C libriv clean
	$(MAKE) -C rivcm clean
	$(MAKE) -C demos clean

.PHONY: rootfs toolchain libriv bwrapbox