update-bindings:
	nelua-lua genbindings.lua

update-libs:
	wget -O miniaudio.h https://raw.githubusercontent.com/mackron/miniaudio/master/miniaudio.h
	wget -O sokol_gfx.h https://raw.githubusercontent.com/floooh/sokol/pre-separate-samplers/sokol_gfx.h
	wget -O sokol_app.h https://raw.githubusercontent.com/floooh/sokol/pre-separate-samplers/sokol_app.h
	wget -O sha3.c https://raw.githubusercontent.com/brainhub/SHA3IUF/master/sha3.c
	wget -O miniz-3.0.2.zip https://github.com/richgel999/miniz/releases/download/3.0.2/miniz-3.0.2.zip
	unzip -o miniz-3.0.2.zip miniz.h miniz.c && rm -f miniz-3.0.2.zip
