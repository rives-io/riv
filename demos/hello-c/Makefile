NAME=hello-c
DATA_FILES=hello.c
RIVEMU=rivemu
RIVEMU_EXEC=$(RIVEMU) -quiet -no-window -sdk -workspace -exec

build: $(NAME).sqfs

run: $(NAME).sqfs
	$(RIVEMU) $<

clean:
	rm -f *.sqfs

$(NAME).sqfs: $(DATA_FILES)
	$(RIVEMU_EXEC) riv-mksqfs $^ $@
