CC=gcc
CFLAGS=-Wall -Werror
LDFLAGS=-Wl,-O1,--sort-common,--build-id=none -z relro -z now
NELUA_FLAGS=-Pnogc --verbose
PREFIX=/usr/local

CARTESI_MACHINE_PREFIX=/opt/cartesi
CARTESI_MACHINE_LIB_DIR=$(CARTESI_MACHINE_PREFIX)/lib/cartesi-machine
CARTESI_MACHINE_INC_DIR=$(CARTESI_MACHINE_PREFIX)/include

INCS=-I"$(CARTESI_MACHINE_INC_DIR)"
LIBS=-L"$(CARTESI_MACHINE_LIB_DIR)" -Wl,-rpath,"$(CARTESI_MACHINE_LIB_DIR)"
NELUA_LIBS=-L../libriv -L../deps

# for release
# NELUA_FLAGS+= -Pnochecks -Pnoassertloc -Pnocstaticassert --release

rivcm: rivcm.nelua *.nelua
	nelua $(NELUA_FLAGS) $(NELUA_LIBS) \
		--cc=$(CC) \
		--cflags="$(CFLAGS) $(INCS)" \
		--ldflags="$(LDFLAGS) $(LIBS)" \
		--cache-dir . \
		--output $@ $<

clean:
	rm -f rivcm rivcm.c

install: rivcm
	mkdir -p $(DESTDIR)$(PREFIX)/rivcm
	install -m755 rivcm $(DESTDIR)$(PREFIX)/bin/rivcm
