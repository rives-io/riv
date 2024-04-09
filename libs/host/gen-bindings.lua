local nldecl = require 'nelua.plugins.nldecl'

-- cartesi
nldecl.generate_bindings_file{
  output_file = 'cartesi.nelua',
  includes = {'cartesi.h'},
  include_dirs = {'.'},
  include_names = {'^cm', '^CM'},
  output_head = [==[
##[[
cinclude 'cartesi.h'
linklib 'cartesi'
]]
]==]
}

-- miniaudio
nldecl.generate_bindings_file{
  output_file = 'miniaudio.nelua',
  defines = {
    'MA_NO_PTHREAD_IN_HEADER',
    'MA_NO_GENERATION',
    'MA_NO_RESOURCE_MANAGER',
  },
  includes = {'miniaudio.h'},
  include_dirs = {'.'},
  include_names = {'^ma_', '^MA_'},
  output_head = [==[
##[[
if not MINIAUDIO_NO_IMPL then
  cdefine 'MA_API static'
  cdefine 'MA_PRIVATE static'
  cdefine 'MA_NO_PTHREAD_IN_HEADER'
  cdefine 'MA_NO_GENERATION'
  cdefine 'MA_NO_RESOURCE_MANAGER'
  cdefine 'MINIAUDIO_IMPLEMENTATION'
end
cinclude 'miniaudio.h'
if ccinfo.is_linux then
  linklib 'dl'
  cflags '-pthread'
elseif ccinfo.is_windows then
  linklib 'ole32'
end
]]
]==]
}

-- sokol
nldecl.generate_bindings_file{
  output_file = 'sokol.nelua',
  includes = {'sokol_gfx.h', 'sokol_gp.h', 'sokol_app.h', 'sokol_args.h', 'sokol_log.h', 'sokol_glue.h'},
  include_dirs = {'.'},
  include_names = {
    '^sg_', '^SG_',
    '^sapp_', '^SAPP_',
    '^sgp_', '^SGP_',
    '^sargs_', '^SARGS_',
    '^slog_', '^SLOG_',
    '^sglue_', '^SGLUE_',
  },
  output_head =
[==[
##[[
-- sokol backend
if ccinfo.is_emscripten then
  cdefine 'SOKOL_GLES3'
elseif ccinfo.is_windows then
  cdefine 'SOKOL_D3D11'
else
  cdefine 'SOKOL_GLCORE33'
end
-- sokol_gfx
cdefine 'SOKOL_GFX_API_DECL static'
cdefine 'SOKOL_GFX_IMPL'
cinclude 'sokol_gfx.h'
if ccinfo.is_windows then
  linklib 'gdi32'
  linklib 'opengl32'
elseif not ccinfo.is_emscripten then
  linklib 'GL'
end
-- sokol_gp
cdefine 'SOKOL_GP_API_DECL static'
cdefine 'SOKOL_GP_IMPL'
cinclude 'sokol_gp.h'
-- sokol_app
cdefine 'SOKOL_APP_API_DECL static'
cdefine 'SOKOL_APP_IMPL'
cdefine 'SOKOL_NO_ENTRY'
cinclude 'sokol_app.h'
if ccinfo.is_linux then
  linklib 'X11'
  linklib 'Xi'
  linklib 'Xcursor'
  linklib 'dl'
  cflags '-pthread'
elseif ccinfo.is_windows then
  linklib 'kernel32'
  linklib 'user32'
  linklib 'shell32'
end
-- sokol_glue
cdefine 'SOKOL_GLUE_API_DECL static'
cdefine 'SOKOL_GLUE_IMPL'
cinclude 'sokol_glue.h'
-- sokol_args
cdefine 'SOKOL_ARGS_API_DECL static'
cdefine 'SOKOL_ARGS_IMPL'
cinclude 'sokol_args.h'
-- sokol_log
cdefine 'SOKOL_LOG_API_DECL static'
cdefine 'SOKOL_LOG_IMPL'
cinclude 'sokol_log.h'
]]
]==]
}
