NAME=jit

COMP?=xz
ASSETS_DIR?=assets
ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

all: build/$(NAME).sqfs build/$(NAME).hash.txt

run: build/$(NAME).sqfs
	cd ../.. && ./rivemu/rivemu -cartridge=$(ROOT_DIR)/build/$(NAME).sqfs $(ARGS)

clean:
	rm -rf build

build/%.sqfs: build/%.fs
	mksquashfs $< $@ \
	    -quiet \
	    -comp $(COMP) \
	    -mkfs-time 0 \
	    -all-time 0 \
	    -all-root \
	    -noappend \
	    -no-fragments \
	    -no-exports \
	    -no-progress

build:
	mkdir -p build

build/%.fs: $(ASSETS_DIR)/*
	rm -rf $@
	mkdir -p $@
	cp -r $(ASSETS_DIR)/* $@/

build/%.hash.txt: build/%.sqfs
	stat -c "%s %n" $^ > $@
	sha1sum $^ >> $@

$(ASSETS_DIR)/*:;

.PHONY: all run clean
.PRECIOUS: build/%.fs
