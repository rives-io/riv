SKEL_FILES=$(shell find skel -type f)
TOOL_FILES=$(shell find ../tools -type f)
LIBRIV_FILES=../libriv/*.nelua ../libriv/*.h
BWRAPBOX_FILES=../bwrapbox/*.nelua ../bwrapbox/seccomp-filter-rules.h
DOCKER_RUN_FLAGS=--platform=linux/riscv64 \
	--volume `pwd`/..:/mnt \
	--workdir /mnt \
	--hostname toolchain-env \
	-it --rm riv/toolchain:devel
DOCKER_USER_RUN_FLAGS=\
	--env USER=$$(id -u -n) --env GROUP=$$(id -g -n) \
	--env UID=$$(id -u) --env GID=$$(id -g) \
	$(DOCKER_RUN_FLAGS)

all: rivos rivos-sdk

rivos: rivos.ext2.zz rivos.ext2 rivos.hash.txt
rivos-sdk: rivos-sdk.ext2 rivos-sdk.hash.txt

rivos.ext2 rivos-sdk.ext2: rivos-sdk.Dockerfile ../kernel/linux-headers.tar.xz $(SKEL_FILES) $(TOOL_FILES) $(LIBRIV_FILES)
	$(MAKE) toolchain
	docker create --platform=linux/riscv64 --name riv-copy-rivos riv/toolchain:devel
	docker cp riv-copy-rivos:/rivos.ext2 .
	docker cp riv-copy-rivos:/rivos-sdk.ext2  .
	docker rm riv-copy-rivos
	touch rivos.ext2 rivos-sdk.ext2

%.ext2.zz: %.ext2
	cat $< | pigz -cz -11 > $@

%.hash.txt: %.ext2
	stat -c "%s %n" $< > $@
	sha256sum $< >> $@

shell: rivos.ext2
	cartesi-machine \
		--append-init=USER=root \
		--flash-drive=label:root,filename:$< \
		--ram-image=../kernel/linux.bin \
		--ram-length=256Mi \
		-it exec /bin/ash -l

shell-devel: rivos-sdk.ext2
	cartesi-machine \
		--append-init=USER=root \
		--flash-drive=label:root,filename:$< \
		--ram-image=../kernel/linux.bin \
		--ram-length=384Mi \
		-n \
		-it exec /bin/ash -l

toolchain:
	docker build --tag riv/toolchain:devel --file rivos-sdk.Dockerfile --progress plain ..

toolchain-exec:
	docker run $(DOCKER_USER_RUN_FLAGS) /bin/bash -c "$(COMMAND)"

toolchain-env:
	docker run $(DOCKER_USER_RUN_FLAGS)

toolchain-env-asroot:
	docker run $(DOCKER_RUN_FLAGS)

clean:
	rm -f *.zz *.ext2 *.hash.txt

# External deps
../kernel/linux.bin ../kernel/linux-headers.tar.xz:
	$(MAKE) -C ../kernel
