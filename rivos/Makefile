SKEL_FILES=$(shell find skel -type f) $(shell find skel-sdk -type f)
LIBRIV_FILES=../libriv/*.nelua ../libriv/*.h

all: rivos rivos-sdk

rivos: rivos.ext2.zz rivos.ext2 rivos.hash.txt
rivos-sdk: rivos-sdk.ext2 rivos-sdk.hash.txt

rivos-sdk.ext2: riv-toolchain-riscv64.Dockerfile ../kernel/linux-headers.tar.xz $(SKEL_FILES) $(LIBRIV_FILES)
	$(MAKE) toolchain
	docker create --platform=linux/riscv64 --name riv-copy-rivos riv/toolchain-riscv64
	docker cp riv-copy-rivos:/rivos.ext2 .
	docker cp riv-copy-rivos:/rivos-sdk.ext2  .
	docker rm riv-copy-rivos
	touch rivos.ext2 rivos-sdk.ext2

rivos.ext2: rivos-sdk.ext2

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

shell-sdk: rivos-sdk.ext2
	cartesi-machine \
		--append-init=USER=root \
		--flash-drive=label:root,filename:$< \
		--ram-image=../kernel/linux.bin \
		--ram-length=384Mi \
		--network \
		-it exec /bin/ash -l

toolchain:
	docker build --tag riv/toolchain-riscv64 --file riv-toolchain-riscv64.Dockerfile --progress plain ..

toolchain-exec:
	docker run \
		--platform=linux/riscv64 \
		--volume "`pwd`/..":/mnt \
		--workdir /mnt \
		--hostname riv-toolchain-riscv64 \
		--rm riv/toolchain-riscv64 /bin/bash -c "$(COMMAND)"

toolchain-env:
	docker run \
		--platform=linux/riscv64 \
		--volume "`pwd`/..":/mnt \
		--workdir /mnt \
		--env USER=$(shell id -u -n) --env GROUP=$(shell id -g -n) \
		--env UID=$(shell id -u) --env GID=$(shell id -g) \
		--hostname riv-toolchain-riscv64 \--rm -it riv/toolchain-riscv64

toolchain-env-asroot:
	docker run \
		--platform=linux/riscv64 \
		--volume "`pwd`/..":/mnt \
		--workdir /mnt \
		--hostname riv-toolchain-riscv64 \
		--rm -it riv/toolchain-riscv64

clean:
	rm -f *.zz *.ext2 *.hash.txt

distclean: clean
	docker rmi riv/toolchain-riscv64

# External deps
../kernel/linux.bin ../kernel/linux-headers.tar.xz:
	$(MAKE) -C ../kernel
