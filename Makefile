all: rootfs rivcm demos

rootfs rivcm:
	$(MAKE) -C $@

libriv bwrapbox demos:
	$(MAKE) -C rootfs toolchain-exec COMMAND="make -C $@"

toolchain toolchain-exec toolchain-env toolchain-env-asroot shell:
	$(MAKE) -C rootfs $@

clean:
	$(MAKE) -C bwrapbox clean
	$(MAKE) -C rootfs clean
	$(MAKE) -C libriv clean
	$(MAKE) -C rivcm clean
	$(MAKE) -C demos clean

.PHONY: rootfs rivcm libriv bwrapbox demos
