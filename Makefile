all:
	$(MAKE) kernel
	$(MAKE) rivos
	$(MAKE) rivemu
	$(MAKE) demos

all-cross:
	$(MAKE) kernel
	$(MAKE) rivos-cross
	$(MAKE) rivemu
	$(MAKE) demos-cross

# Targets that uses the host toolchain
libs kernel rivos rivemu rivemu-web:
	$(MAKE) -C $@

libriv-cross:
	$(MAKE) -C libriv

tools-cross:
	$(MAKE) -C tools

rivos-cross: libriv-cross tools-cross
	$(MAKE) -C rivos

demos-cross:
	$(MAKE) -C demos

# Targets that uses RISC-V toolchain
libriv tools demos:
	$(MAKE) -C rivos toolchain-exec COMMAND="make -C $@"

toolchain toolchain-exec toolchain-env toolchain-env-asroot shell shell-devel:
	$(MAKE) -C rivos $@

update-libs:
	$(MAKE) -C libs update-libs

update-bindings:
	$(MAKE) -C libriv update-bindings
	$(MAKE) -C libs update-bindings

clean:
	$(MAKE) -C rivos clean
	$(MAKE) -C libriv clean
	$(MAKE) -C rivemu clean
	$(MAKE) -C rivemu-web clean
	$(MAKE) -C demos clean
	$(MAKE) -C tools clean
	$(MAKE) -C libs clean

distclean: clean
	$(MAKE) -C kernel distclean

.PHONY: kernel rivos demos rivemu libs libriv tools rivemu-web

##################
# Demo testing
DEMO=snake

demo:
	$(MAKE) -C rivos toolchain-exec COMMAND="make -C demos/$(DEMO) -j4"

demo-clean:
	$(MAKE) -C rivos toolchain-exec COMMAND="make -C demos/$(DEMO) clean"

demo-run:
	$(MAKE) -C demos/$(DEMO) run
