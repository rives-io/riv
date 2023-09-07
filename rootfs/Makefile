SKEL_FILES=$(shell find skel -type f)
TOOL_FILES=$(shell find ../tools -type f)
LIBRIV_FILES=../libriv/*.nelua ../libriv/*.h
BWRAPBOX_FILES=../bwrapbox/*.nelua ../bwrapbox/seccomp-filter-rules.h
DOCKER_RUN_FLAGS=--platform=linux/riscv64 \
	-v `pwd`/..:/mnt -w /mnt \
	--hostname toolchain-env -it --rm riv/toolchain:devel
DOCKER_USER_RUN_FLAGS=\
	-e USER=$$(id -u -n) -e GROUP=$$(id -g -n) \
	-e UID=$$(id -u) -e GID=$$(id -g) \

all: rootfs

rootfs: rootfs.ext2 rootfs.hash.txt

rootfs.ext2: toolchain-rootfs.Dockerfile $(SKEL_FILES) $(TOOL_FILES) $(LIBRIV_FILES)
	$(MAKE) toolchain
	docker create --platform=linux/riscv64 --name riv-copy-rootfs riv/toolchain:devel
	docker cp riv-copy-rootfs:/$@ .
	docker rm riv-copy-rootfs
	touch $@

rootfs.hash.txt: rootfs.ext2
	stat -c "%s %n" $< > $@
	sha1sum $< >> $@

shell: rootfs.ext2
	cartesi-machine \
		--flash-drive=label:root,filename:$< \
		--ram-image=../kernel/linux.bin \
		--rom-image=../kernel/rom.bin \
		--append-rom-bootargs="TERM=$(TERM) LINES=$(shell tput lines) COLUMNS=$(shell tput cols)" \
		--htif-yield-automatic \
		-i exec /bin/ash

toolchain:
	docker build --target toolchain-rootfs-stage --tag riv/toolchain:devel --file toolchain-rootfs.Dockerfile --progress plain ..

toolchain-exec:
	docker run $(DOCKER_USER_RUN_FLAGS) $(DOCKER_RUN_FLAGS) /bin/bash -c "$(COMMAND)"

toolchain-env:
	docker run $(DOCKER_USER_RUN_FLAGS) $(DOCKER_RUN_FLAGS)

toolchain-env-asroot:
	docker run $(DOCKER_RUN_FLAGS)

clean:
	rm -f rootfs.ext2 rootfs.hash.txt
