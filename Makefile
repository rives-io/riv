all:
	$(MAKE) kernel
	$(MAKE) rootfs
	$(MAKE) demos
	$(MAKE) rivcm

# Targets that uses the host toolchain
deps kernel rootfs rivcm:
	$(MAKE) -C $@

# Targets that uses RISC-V toolchain
libriv bwrapbox demos tools:
	$(MAKE) -C rootfs toolchain-exec COMMAND="make -C $@"

toolchain toolchain-exec toolchain-env toolchain-env-asroot shell:
	$(MAKE) -C rootfs $@

update-libs:
	$(MAKE) -C deps update-libs

update-bindings:
	$(MAKE) -C libriv update-bindings
	$(MAKE) -C deps update-bindings

clean:
	$(MAKE) -C bwrapbox clean
	$(MAKE) -C rootfs clean
	$(MAKE) -C libriv clean
	$(MAKE) -C rivcm clean
	$(MAKE) -C demos clean
	$(MAKE) -C tools clean
	$(MAKE) -C deps clean

distclean: clean
	$(MAKE) -C kernel distclean

.PHONY: kernel rootfs demos rivcm deps libriv bwrapbox tools
