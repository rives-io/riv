include ../demo-base.mk

NELUA_FLAGS+=-Pnochecks -Pnoerrorloc -Pnocstaticassert --verbose
# NELUA_FLAGS+=-Pnocfeaturessetup -Pnocwarnpragmas
NELUA_FLAGS+=-L../../libriv/?.nelua

$(NAME).elf: $(NAME).nelua *.nelua ../../libriv/*.nelua ../../libriv/*.h
	nelua $(NELUA_FLAGS) \
		--cc=$(CC) \
		--cflags="$(CFLAGS)" \
		--ldflags="$(LDFLAGS)" \
		--cache-dir . \
		--binary \
		--output $@ $<
	touch $@

clean::
	rm -f $(NAME).c
