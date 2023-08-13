update-bindings:
	cd guest-host && nelua-lua gen-bindings.lua
	cd host && nelua-lua gen-bindings.lua

update-libs:
	wget -O host/miniaudio.h https://raw.githubusercontent.com/mackron/miniaudio/master/miniaudio.h
	wget -O host/sokol_gfx.h https://raw.githubusercontent.com/floooh/sokol/pre-separate-samplers/sokol_gfx.h
	wget -O host/sokol_app.h https://raw.githubusercontent.com/floooh/sokol/pre-separate-samplers/sokol_app.h
	wget -O guest-host/sha3.c https://raw.githubusercontent.com/brainhub/SHA3IUF/master/sha3.c
	wget -O guest-host/miniz-3.0.2.zip https://github.com/richgel999/miniz/releases/download/3.0.2/miniz-3.0.2.zip
	cd guest-host/ && unzip -o miniz-3.0.2.zip miniz.h miniz.c && rm -f miniz-3.0.2.zip
