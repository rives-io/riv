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
linklib 'cartesi-0.15'
]]
]==]
}

-- miniaudio
nldecl.generate_bindings_file{
  output_file = 'miniaudio.nelua',
  defines = {'MA_NO_PTHREAD_IN_HEADER'},
  includes = {'miniaudio.h'},
  include_dirs = {'.'},
  include_names = {'^ma_', '^MA_'},
  output_head = [==[
##[[
if not MINIAUDIO_NO_IMPL then
  cdefine 'MA_API static'
  cdefine 'MA_PRIVATE static'
  cdefine 'MA_NO_PTHREAD_IN_HEADER'
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
  includes = {'sokol_gfx.h', 'sokol_gp.h', 'sokol_app.h'},
  include_dirs = {'.'},
  include_names = {
    '^sg_', '^SG_',
    '^sapp_', '^SAPP_',
    '^sgp_', '^SGP_',
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
if not SOKOL_GFX_NO_IMPL then
  cdefine 'SOKOL_GFX_API_DECL static'
  cdefine 'SOKOL_GFX_IMPL'
end
cinclude 'sokol_gfx.h'
if ccinfo.is_windows then
  linklib 'gdi32'
  linklib 'opengl32'
else
  linklib 'GL'
end
-- sokol_gp
if not SOKOL_GP_NO_IMPL then
  cdefine 'SOKOL_GP_API_DECL static'
  cdefine 'SOKOL_GP_IMPL'
end
cinclude 'sokol_gp.h'
-- sokol_app
if not SOKOL_APP_NO_IMPL then
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
end
]]
]==],
  output_foot =
[==[
global function sapp_sgcontext(): sg_context_desc
  local desc: sg_context_desc
  desc.color_format = (@sg_pixel_format)(sapp_color_format())
  desc.depth_format = (@sg_pixel_format)(sapp_depth_format())
  desc.sample_count = sapp_sample_count()
  desc.metal.device = sapp_metal_get_device()
  desc.metal.renderpass_descriptor_cb = sapp_metal_get_renderpass_descriptor
  desc.metal.drawable_cb = sapp_metal_get_drawable
  desc.d3d11.device = sapp_d3d11_get_device()
  desc.d3d11.device_context = sapp_d3d11_get_device_context()
  desc.d3d11.render_target_view_cb = sapp_d3d11_get_render_target_view
  desc.d3d11.depth_stencil_view_cb = sapp_d3d11_get_depth_stencil_view
  desc.wgpu.device = sapp_wgpu_get_device()
  desc.wgpu.render_view_cb = sapp_wgpu_get_render_view
  desc.wgpu.resolve_view_cb = sapp_wgpu_get_resolve_view
  desc.wgpu.depth_stencil_view_cb = sapp_wgpu_get_depth_stencil_view
  return desc
end
]==]
}
