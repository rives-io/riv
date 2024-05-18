local nldecl = require 'nelua.plugins.nldecl'

-- cartesi
nldecl.generate_bindings_file{
  output_file = 'cartesi.nelua',
  includes = {'machine-c-api.h'},
  include_dirs = {'.'},
  include_names = {'^cm', '^CM'},
  output_head = [==[
##[[
cinclude 'machine-c-api.h'
if CARTESI_STATIC then
  if ccinfo.is_macos then
    linklib 'cartesi'
    linklib 'c++'
  elseif ccinfo.is_wasm then
    linklib 'libcartesi.a'
  elseif ccinfo.is_windows then
    linklib 'libcartesi.a'
    linklib 'libstdc++.a'
    linklib 'ws2_32'
  elseif ccinfo.is_linux then
    linklib 'libcartesi.a'
    linklib 'libstdc++.a'
    linklib 'libslirp.a'
    linklib 'libglib-2.0.a'
  end
else
  linklib 'cartesi'
end
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
  cdefine 'MA_NO_GENERATION'
  cdefine 'MA_NO_RESOURCE_MANAGER'
  cdefine 'MINIAUDIO_IMPLEMENTATION'
end
if ccinfo.is_macos then
  cdefine 'MA_NO_RUNTIME_LINKING'
end
cinclude 'miniaudio.h'
if ccinfo.is_linux then
  linklib 'dl'
  cflags '-pthread'
elseif ccinfo.is_windows then
  linklib 'ole32'
elseif ccinfo.is_macos then
  ldflags '-framework CoreFoundation'
  ldflags '-framework CoreAudio'
  ldflags '-framework AudioToolbox'
  linklib 'pthread'
  linklib 'm'
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
  linklib 'gdi32'
  linklib 'dxgi'
  linklib 'd3d11'
elseif ccinfo.is_macos then
  cdefine 'SOKOL_METAL'
else
  cdefine 'SOKOL_GLCORE33'
  linklib 'GL'
end
-- sokol_gfx
if not ccinfo.is_macos then
  cdefine 'SOKOL_GFX_API_DECL static'
  cdefine 'SOKOL_GFX_IMPL'
end
cinclude 'sokol_gfx.h'
-- sokol_gp
if not ccinfo.is_macos then
  cdefine 'SOKOL_GP_API_DECL static'
  cdefine 'SOKOL_GP_IMPL'
end
cinclude 'sokol_gp.h'
-- sokol_app
if not ccinfo.is_macos then
  cdefine 'SOKOL_APP_API_DECL static'
  cdefine 'SOKOL_APP_IMPL'
end
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
elseif ccinfo.is_macos then
  ldflags '-framework Cocoa'
  ldflags '-framework QuartzCore'
  ldflags '-framework Metal'
  ldflags '-framework MetalKit'
  local fs = require 'nelua.utils.fs'
  local sokol_m = fs.realpath(fs.scriptdir()..'/sokol.m')
  cflags('-x objective-c '..sokol_m..' -x c')
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
