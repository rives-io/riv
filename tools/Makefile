TOOLS=riv-unpacklog
MAKETARGET=

all: $(TOOLS)

$(TOOLS):
	$(MAKE) -C $@ $(MAKETARGET)

install: MAKETARGET=install
install: $(TOOLS)

clean: MAKETARGET=clean
clean: $(TOOLS)

.PHONY: $(TOOLS)
