NAME=hello-js
DATA_FILES=hello.js
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
	rm -f *.sqfs

$(NAME).sqfs: $(DATA_FILES)
	$(RIVEMU_EXEC) riv-mksqfs $^ $@
