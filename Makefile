all: rootfs rivcm demos

rootfs rivcm deps:
	$(MAKE) -C $@

libriv bwrapbox demos tools:
	$(MAKE) -C rootfs toolchain-exec COMMAND="make -C $@"

toolchain toolchain-exec toolchain-env toolchain-env-asroot shell:
	$(MAKE) -C rootfs $@

clean:
	$(MAKE) -C bwrapbox clean
	$(MAKE) -C rootfs clean
	$(MAKE) -C libriv clean
	$(MAKE) -C rivcm clean
	$(MAKE) -C demos clean
	$(MAKE) -C tools clean

update-bindings:
	$(MAKE) -C libriv update-bindings
	$(MAKE) -C deps update-bindings

.PHONY: rootfs rivcm libriv bwrapbox demos tools deps
