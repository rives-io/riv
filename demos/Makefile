DEMOS=$(sort $(subst /,,$(wildcard */)))
TARGET=

all: $(DEMOS)

$(DEMOS):
	$(MAKE) -C $@ $(TARGET)

clean: TARGET=clean
clean: $(DEMOS)

.PHONY: $(DEMOS)
