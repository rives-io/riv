SOURCES=hello.rs
EXE=hello.elf
NAME=hello-rs
RIVEMU_RUN=rivemu
RIVEMU_EXEC=rivemu -quiet -no-window -sdk -workspace -exec
ifneq (,$(wildcard /usr/sbin/riv-run))
	RIVEMU_RUN=riv-run
	RIVEMU_EXEC=
endif

build: $(NAME).sqfs

run: $(NAME).sqfs
	$(RIVEMU_RUN) $<

clean:
	rm -f *.sqfs *.elf *.o libriv.so riv.h

$(NAME).sqfs: $(EXE)
	$(RIVEMU_EXEC) riv-mksqfs $^ $@

$(EXE): $(SOURCES)
	$(RIVEMU_EXEC) cp /usr/lib/libriv.so .
	rustc -o $@ $^ \
		-C opt-level=z \
		-C lto=true \
		-C panic=abort \
		-C strip=symbols \
		-C linker=riscv64-buildroot-linux-musl-gcc \
		-L. \
		-lriv \
		--target=riscv64gc-unknown-linux-musl
	$(RIVEMU_EXEC) riv-strip $@
	rm -f libriv.so
