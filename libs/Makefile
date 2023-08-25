MINIZ_VERSION=3.0.2
CARTESI_VERSION=0.15.1
SOKOL_TAG=pre-separate-samplers
MINIAUDIO_BRANCH=master

all: | update-libs update-bindings

update-libs: update-miniz update-sha3 update-sokol update-miniaudio update-cartesi

update-bindings:
	cd guest-host && nelua-lua gen-bindings.lua
	cd host && nelua-lua gen-bindings.lua

update-miniz:
	wget -O guest-host/miniz-$(MINIZ_VERSION).zip https://github.com/richgel999/miniz/releases/download/$(MINIZ_VERSION)/miniz-$(MINIZ_VERSION).zip
	cd guest-host && unzip -o miniz-$(MINIZ_VERSION).zip miniz.h miniz.c && rm -f miniz-$(MINIZ_VERSION).zip

update-sha3:
	wget -O guest-host/sha3.c https://raw.githubusercontent.com/brainhub/SHA3IUF/master/sha3.c

update-sokol:
	wget -O host/sokol_gfx.h https://raw.githubusercontent.com/floooh/sokol/$(SOKOL_TAG)/sokol_gfx.h
	wget -O host/sokol_app.h https://raw.githubusercontent.com/floooh/sokol/$(SOKOL_TAG)/sokol_app.h
	wget -O host/sokol_args.h https://raw.githubusercontent.com/floooh/sokol/$(SOKOL_TAG)/sokol_args.h

update-miniaudio:
	wget -O host/miniaudio.h https://raw.githubusercontent.com/mackron/miniaudio/$(MINIAUDIO_BRANCH)/miniaudio.h

update-cartesi:
	wget -O cartesi-machine-v$(CARTESI_VERSION)_amd64.deb https://github.com/cartesi/machine-emulator/releases/download/v$(CARTESI_VERSION)/cartesi-machine-v$(CARTESI_VERSION)_amd64.deb
	dpkg-deb -x cartesi-machine-v$(CARTESI_VERSION)_amd64.deb cartesi-machine-pkg
	cat cartesi-machine-pkg/usr/include/cartesi-machine/machine-c-version.h > host/cartesi.h
	cat cartesi-machine-pkg/usr/include/cartesi-machine/machine-c-defines.h >> host/cartesi.h
	cat cartesi-machine-pkg/usr/include/cartesi-machine/machine-c-api.h >> host/cartesi.h
	sed -i '/#include "/d' host/cartesi.h
	rm -rf cartesi-machine-pkg cartesi-machine-v$(CARTESI_VERSION)_amd64.deb

clean:
	rm -rf cartesi-machine-pkg cartesi-machine-v$(CARTESI_VERSION)_amd64.deb miniz-$(MINIZ_VERSION).zip
