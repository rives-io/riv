SOURCES=hello.zig
EXE=hello.elf
NAME=hello-zig
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
	$(RIVEMU_EXEC) cp /usr/include/riv.h /usr/lib/libriv.so .
	zig build-exe $^ -femit-bin=$@ -fsingle-threaded -fstrip \
		-target riscv64-linux-musl \
		-O ReleaseSmall -I.	libriv.so -lc -dynamic
	$(RIVEMU_EXEC) riv-strip $@
	rm -f libriv.so riv.h
