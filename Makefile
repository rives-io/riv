all:
	$(MAKE) kernel
	$(MAKE) rivos
	$(MAKE) rivemu
	$(MAKE) demos

# Targets that uses the host toolchain
libs kernel rivos rivemu rivemu-web demos:
	$(MAKE) -C $@

libriv-cross:
	$(MAKE) -C libriv

# Targets that uses RISC-V toolchain
libriv:
	$(MAKE) -C rivos toolchain-exec COMMAND="make -C $@"

toolchain toolchain-exec toolchain-env toolchain-env-asroot shell shell-sdk:
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
	$(MAKE) -C libs clean

distclean: clean
	$(MAKE) -C kernel distclean
	$(MAKE) -C rivemu distclean
	$(MAKE) -C rivos distclean
	rm -rf dist

.PHONY: kernel rivos demos rivemu libs libriv rivemu-web

##################
# Demo testing
DEMO=snake

demo:
	$(MAKE) -C rivos toolchain-exec COMMAND="make -C demos/$(DEMO)"

demo-clean:
	$(MAKE) -C rivos toolchain-exec COMMAND="make -C demos/$(DEMO) clean"

demo-run:
	$(MAKE) -C demos/$(DEMO) run

.PHONY: dist
dist:
	$(MAKE) -C kernel
	$(MAKE) -C rivos
	$(MAKE) -C rivemu dist
	rm -rf dist
	mkdir -p dist
	cp rivos/rivos.ext2 rivos/rivos-sdk.ext2 dist/
	cp rivemu/rivemu.js rivemu/rivemu.wasm rivemu/rivemu-linux-amd64 rivemu/rivemu-linux-arm64 rivemu/rivemu-windows-amd64.exe dist/
