MINIZ_VERSION=3.0.2
CARTESI_VERSION=feature/optim-fetch
SOKOL_TAG=master
SHA3_BRANCH=master
MINIAUDIO_BRANCH=master
SPNG_BRANCH=master

all: | update-libs update-bindings

update-libs: update-miniz update-sha256 update-sokol update-sokol_gp update-spng update-miniaudio update-cartesi

update-bindings:
	cd guest-host && nelua-lua gen-bindings.lua
	cd host && nelua-lua gen-bindings.lua

update-miniz:
	wget -O guest-host/miniz-$(MINIZ_VERSION).zip https://github.com/richgel999/miniz/releases/download/$(MINIZ_VERSION)/miniz-$(MINIZ_VERSION).zip
	cd guest-host && unzip -o miniz-$(MINIZ_VERSION).zip miniz.h miniz.c && rm -f miniz-$(MINIZ_VERSION).zip

update-sha256:
	wget -O guest-host/sha256.h https://raw.githubusercontent.com/ilvn/SHA256/main/sha256.h
	wget -O guest-host/sha256.c https://raw.githubusercontent.com/ilvn/SHA256/main/sha256.c

update-sokol:
	wget -O host/sokol_gfx.h https://raw.githubusercontent.com/floooh/sokol/$(SOKOL_TAG)/sokol_gfx.h
	wget -O host/sokol_app.h https://raw.githubusercontent.com/floooh/sokol/$(SOKOL_TAG)/sokol_app.h
	wget -O host/sokol_args.h https://raw.githubusercontent.com/floooh/sokol/$(SOKOL_TAG)/sokol_args.h
	wget -O host/sokol_log.h https://raw.githubusercontent.com/floooh/sokol/$(SOKOL_TAG)/sokol_log.h
	wget -O host/sokol_glue.h https://raw.githubusercontent.com/floooh/sokol/$(SOKOL_TAG)/sokol_glue.h

update-sokol_gp:
	wget -O host/sokol_gp.h https://raw.githubusercontent.com/edubart/sokol_gp/$(SOKOL_TAG)/sokol_gp.h

update-spng:
	wget -O guest-host/spng.h https://raw.githubusercontent.com/randy408/libspng/$(SPNG_BRANCH)/spng/spng.h
	wget -O guest-host/spng.c https://raw.githubusercontent.com/randy408/libspng/$(SPNG_BRANCH)/spng/spng.c

update-miniaudio:
	wget -O host/miniaudio.h https://raw.githubusercontent.com/mackron/miniaudio/$(MINIAUDIO_BRANCH)/miniaudio.h

update-cartesi:
	wget -O host/machine-c-api.h https://raw.githubusercontent.com/cartesi/machine-emulator/$(CARTESI_VERSION)/src/machine-c-api.h
	wget -O host/machine-c-defines.h https://raw.githubusercontent.com/cartesi/machine-emulator/$(CARTESI_VERSION)/src/machine-c-defines.h
	wget -O host/machine-c-version.h https://raw.githubusercontent.com/cartesi/machine-emulator/$(CARTESI_VERSION)/tools/template/machine-c-version.h.template
# 	sed -i 's/EMULATOR_VERSION_MAJOR/0/g;s/EMULATOR_VERSION_MINOR/0/g;s/EMULATOR_VERSION_MINOR/17/g;s/EMULATOR_VERSION_MINOR/17/g;s/EMULATOR_VERSION_LABEL/"-riv"/g' host/machine-c-version.h

clean:
	rm -rf cartesi-machine-pkg cartesi-machine-v$(CARTESI_VERSION)_amd64.deb miniz-$(MINIZ_VERSION).zip
