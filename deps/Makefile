update-bindings:
	cd guest-host && nelua-lua gen-bindings.lua
	cd host && nelua-lua gen-bindings.lua

update-cartesi-c-api:
	wget -O cartesi-machine-v0.15.0_amd64.deb https://github.com/cartesi/machine-emulator/releases/download/v0.15.0/cartesi-machine-v0.15.0_amd64.deb
	dpkg-deb -x cartesi-machine-v0.15.0_amd64.deb cartesi-machine-pkg
	cat cartesi-machine-pkg/usr/include/cartesi-machine/machine-c-version.h > host/cartesi.h
	cat cartesi-machine-pkg/usr/include/cartesi-machine/machine-c-defines.h >> host/cartesi.h
	cat cartesi-machine-pkg/usr/include/cartesi-machine/machine-c-api.h >> host/cartesi.h
	sed -i '/#include "/d' host/cartesi.h
	rm -rf cartesi-machine-pkg cartesi-machine-v0.15.0_amd64.deb

update-libs: update-cartesi-c-api
	wget -O host/miniaudio.h https://raw.githubusercontent.com/mackron/miniaudio/master/miniaudio.h
	wget -O host/sokol_gfx.h https://raw.githubusercontent.com/floooh/sokol/pre-separate-samplers/sokol_gfx.h
	wget -O host/sokol_app.h https://raw.githubusercontent.com/floooh/sokol/pre-separate-samplers/sokol_app.h
	wget -O guest-host/sha3.c https://raw.githubusercontent.com/brainhub/SHA3IUF/master/sha3.c
	wget -O guest-host/miniz-3.0.2.zip https://github.com/richgel999/miniz/releases/download/3.0.2/miniz-3.0.2.zip
	cd guest-host/ && unzip -o miniz-3.0.2.zip miniz.h miniz.c && rm -f miniz-3.0.2.zip

clean:
	rm -rf cartesi-machine-pkg cartesi-machine-v0.15.0_amd64.deb miniz-3.0.2.zip
