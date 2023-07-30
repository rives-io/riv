SKEL_FILES=$(shell find skel -type f)
LIBRIV_FILES=../libriv/*.nelua ../libriv/*.h
BWRAPBOX_FILES=../bwrapbox/*.nelua ../bwrapbox/seccomp-filter-rules.h
DOCKER_RUN_FLAGS=\
	--platform=linux/riscv64 \
	-e USER=$$(id -u -n) -e GROUP=$$(id -g -n) \
	-e UID=$$(id -u) -e GID=$$(id -g) \
	-v `pwd`/..:/mnt -w /mnt \
	--hostname toolchain-env -it --rm riv/toolchain:devel

rootfs.ext2: rootfs.Dockerfile toolchain-rootfs.Dockerfile $(LIBRIV_FILES) $(SKEL_FILES)
	$(MAKE) toolchain-image
	docker build --target rootfs-stage -f $< --progress plain --output . ..
	touch $@
	stat -c "%s %n" $@ > rootfs.hash.txt
	sha1sum $@ >> rootfs.hash.txt
	cat rootfs.hash.txt

rootfs-env: rootfs.ext2
	/opt/cartesi/bin/cartesi-machine \
		--flash-drive=label:root,filename:$< \
		--ram-image=../kernel/linux.bin \
		--rom-image=../rom/rom.bin \
		--append-rom-bootargs="init=/sbin/init TERM=$(TERM) LINES=$(shell tput lines) COLUMNS=$(shell tput cols) RUN_SHELL=1" \
		--htif-yield-automatic \
		-i exec /bin/ash

toolchain-image:
	docker build --target toolchain-rootfs-stage -t riv/toolchain:devel -f toolchain-rootfs.Dockerfile --progress plain ..

toolchain-env:
	docker run $(DOCKER_RUN_FLAGS) /bin/bash

toolchain-exec:
	docker run $(DOCKER_RUN_FLAGS) /bin/bash -c "$(COMMAND)"

clean:
	rm -f rootfs.ext2 rootfs.hash.txt
