local nldecl = require 'nelua.plugins.nldecl'

nldecl.generate_bindings_file{
  output_file = 'riv_types.nelua',
  includes = {'riv.h'},
  include_dirs = {'.'},
  include_names = {'^riv_', '^RIV_'},
  -- import_constants = true,
  force_exclude_names = {
    ['riv_unbounded_uint8'] = true,
    ['riv_unbounded_uint32'] = true,
    ['riv_unbounded_bool'] = true,
  },
  enum_types = {
    riv_key_code = 'uint8',
    riv_color_id = 'uint8',
    riv_mmio_size = 'uint64',
    riv_mmio_start = 'uint64',
    riv_vaddr_base = 'uint64',
  },
  gsubs = {
    {'global function [^\n]+\n', ''},
    -- enums
    {'enum%(cint%)', 'enum(uint32)'},
    -- sized arrays
    {'inoutbuffer: %*', 'inoutbuffer: *[0]'},
    {'framebuffer: %*', 'framebuffer: *[0]'},
    {'audiobuffer: %*', 'audiobuffer: *[0]'},
    {'tracked_keys: %*', 'tracked_keys: *[0]'},
    {'pixels: %*', 'pixels: *[0]'},
    {'data: %*', 'data: *[0]'},
    {'verify_key_events: %*', 'verify_key_events: *[0]'},
    {'palette: %*', 'palette: *[0]'},
  },
  output_head = [[
## pragmas.unitname = ''
## cinclude 'riv.h'
]],
  output_foot = [[
global RIV_DRIVER_MAGIC: riv_magic_buffer <cimport,nodecl>
global RIV_DEVICE_MAGIC: riv_magic_buffer <cimport,nodecl>
global riv: *riv_context <cimport,nodecl>
]]
}
