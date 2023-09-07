all:
	$(MAKE) kernel
	$(MAKE) rootfs
	$(MAKE) rivemu
	$(MAKE) demos

# Targets that uses the host toolchain
libs kernel rootfs rivemu:
	$(MAKE) -C $@

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

.PHONY: kernel rootfs demos rivemu libs libriv tools

##################
# Demo testing
DEMO=snake

demo:
	$(MAKE) -C rootfs toolchain-exec COMMAND="make -C demos/$(DEMO) -j4"

demo-clean:
	$(MAKE) -C rootfs toolchain-exec COMMAND="make -C demos/$(DEMO) clean"

demo-run:
	$(MAKE) -C demos/$(DEMO) run
