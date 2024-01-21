DEMOLANG=nelua
SAMPLES=$(patsubst %.nelua, build/%.sqfs, $(wildcard *.nelua)) \
		$(patsubst %.nelua, build/%.dump.txt, $(wildcard *.nelua)) \
		$(patsubst %.nelua, build/%.readelf.txt, $(wildcard *.nelua)) \
		$(patsubst %.nelua, build/%.strings.txt, $(wildcard *.nelua)) \
		$(patsubst %.nelua, build/%.hash.txt, $(wildcard *.nelua)) \
		$(patsubst %.nelua, build/%.elftoc.c, $(wildcard *.nelua))
all: $(SAMPLES)
include ../demo-base.mk
