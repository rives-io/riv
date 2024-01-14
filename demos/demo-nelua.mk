include ../demo-base.mk

NELUA_FLAGS+=-Pnochecks -Pnoerrorloc -Pnocstaticassert --verbose
# NELUA_FLAGS+=-Pnocfeaturessetup -Pnocwarnpragmas

ifneq ($(HOST_ARCH),riscv64)
	# cross compiling
	DEPS=../../libriv/riv.h ../../libriv/libriv.so ../../libriv/riv.nelua
	NELUA_FLAGS+=-L../../libriv/?.nelua
else
	DEPS=
endif

$(NAME).elf: $(NAME).nelua *.nelua  $(DEPS)
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
