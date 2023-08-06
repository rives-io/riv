local nldecl = require 'nelua.plugins.nldecl'

nldecl.generate_bindings_file{
  output_file = 'riv_types.nelua',
  includes = {'riv.h'},
  include_dirs = {'.'},
  include_names = {'^riv_', '^RIV_'},
  force_exclude_names = {
    ['riv_span_uint8'] = true,
    ['riv_unbounded_uint8'] = true,
    ['riv_unbounded_uint32'] = true,
    ['riv_unbounded_bool'] = true,
  },
  enum_types = {
    riv_key_code = 'uint8',
    riv_pal16_color = 'uint8',
    riv_mem_size = 'uint64',
    riv_mmio_offset = 'uint64',
    riv_vaddr_base = 'uint64',
    riv_paddr_base = 'uint64',
  },
  gsubs = {
    {'global function [^\n]+\n', ''},
    -- enums
    {'enum%(cuint%)', 'enum(uint32)'},
    {'enum%(cint%)', 'enum(uint32)'},
    -- force decl
    {'<cimport,nodecl,', '<'},
    {' <cimport,nodecl>', ''},
    -- sized arrays
    {'framebuffer: %*', 'framebuffer: *[RIV_MEMSIZE_FRAMEBUFFER]'},
    {'audiobuffer: %*', 'audiobuffer: *[RIV_MEMSIZE_AUDIOBUFFER]'},
    {'tracked_keys: %*', 'tracked_keys: *[128]'},
    {'palette: %*', 'palette: *[256]'},
  },
  output_head = [[
## pragmas.unitname = ''
]],
  output_foot = [[
global RIV_DRIVER_MAGIC: [32]byte <const> = {
  0x3f, 0xdf, 0x37, 0x1e, 0xc0, 0xfc, 0xd1, 0xba,
  0xec, 0xe9, 0x72, 0xa1, 0xf5, 0x89, 0x87, 0xc5,
  0x70, 0xfd, 0xbe, 0xc0, 0xce, 0xcc, 0x2d, 0x74,
  0x8d, 0x45, 0x39, 0x62, 0x49, 0xb8, 0x15, 0x26
}
global RIV_DEVICE_MAGIC: [32]byte <const> = {
  0x83, 0x0b, 0x3a, 0xd1, 0xcc, 0x8b, 0xc2, 0xe5,
  0x70, 0x5c, 0x83, 0x98, 0x6c, 0xe4, 0x67, 0xc9,
  0xc1, 0xc6, 0x0b, 0xc6, 0xb9, 0x80, 0xa4, 0x1c,
  0x34, 0x12, 0x8c, 0x2e, 0x05, 0xd8, 0x2c, 0x4e
}
]]
}
