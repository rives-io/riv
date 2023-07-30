DOCKER_RUN_FLAGS=\
	--platform=linux/riscv64 \
	-e USER=$$(id -u -n) -e GROUP=$$(id -g -n) \
	-e UID=$$(id -u) -e GID=$$(id -g) \
	-v `pwd`/..:/mnt -w /mnt \
	--hostname toolchain-env -it --rm riv/toolchain:devel

toolchain-image:
	docker build -t riv/toolchain:devel -f toolchain.Dockerfile --progress plain ..

toolchain-env:
	docker run $(DOCKER_RUN_FLAGS) /bin/bash

toolchain-exec:
	docker run $(DOCKER_RUN_FLAGS) /bin/bash -c "$(COMMAND)"
