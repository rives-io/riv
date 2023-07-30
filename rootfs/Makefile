SKEL_FILES=$(shell find skel -type f)
BWRAPBOX_FILES=../bwrapbox/*.nelua ../bwrapbox/seccomp-filter-rules.h

rootfs.ext2: rootfs.Dockerfile
	docker build -t riv/rootfs:devel -f $< --progress plain --output . ..
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

clean:
	rm -f rootfs.ext2 rootfs.hash.txt
