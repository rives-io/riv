SOURCES=hello.rs
EXE=hello.elf
NAME=hello-rs
RIVEMU_RUN=rivemu
RIVEMU_EXEC=rivemu -quiet -no-window -sdk -workspace -exec
DATA_FILES_PATH=assets/

ifneq (,$(wildcard /usr/sbin/riv-run))
	RIVEMU_RUN=riv-run
	RIVEMU_EXEC=
endif

ifeq (, $(shell which rustc))
$(error "rust is not installed, please install it from https://www.rust-lang.org/tools/install")
endif

ifeq (, $(shell rustup show | grep nightly))
$(error "nightly toolchain is not installed, please install it by running `rustup toolchain install nightly`")
endif

ifeq (, $(shell rustup +nightly target list | grep riscv64gc-unknown-linux-musl))
$(error "riscv64gc-unknown-linux-musl toolchain is not installed, please install it by running `rustup +nightly target add riscv64gc-unknown-linux-musl`")
endif

ifeq (, $(shell which riscv64-buildroot-linux-musl-gcc))
$(error "riscv64-buildroot-linux-musl-gcc is not installed, please get the prebuilt toolchain from https://toolchains.bootlin.com/ (select riscv64-lp64d -> musl) and add it to your PATH")
endif

ifeq (, $(shell which bindgen))
$(error "bindgen(cli) is not installed, please install it by running `cargo install bindgen-cli`")
endif

build: $(NAME).sqfs

run: $(NAME).sqfs
	$(RIVEMU_RUN) $<

clean:
	rm -f *.sqfs *.elf *.o libriv.so riv.h

$(NAME).sqfs: $(EXE)
	$(RIVEMU_EXEC) riv-mksqfs $(DATA_FILES_PATH) $^ $@

$(EXE): $(SOURCES)
	$(RIVEMU_EXEC) cp  /usr/include/riv.h /usr/lib/libriv.so .
	rustup run nightly bindgen riv.h -o riv.rs --rustified-enum ".*" --no-layout-tests --allowlist-file riv.h
	rustc +nightly -o $@ $^ \
		-C opt-level=z \
		-C lto=true \
		-C panic=abort \
		-C strip=symbols \
		-C linker=riscv64-buildroot-linux-musl-gcc \
		-L. \
		-lriv \
		--target=riscv64gc-unknown-linux-musl
	$(RIVEMU_EXEC) riv-strip $@
	rm -f libriv.so riv.h
