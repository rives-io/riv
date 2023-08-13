local nldecl = require 'nelua.plugins.nldecl'

-- sha3
nldecl.generate_bindings_file{
  output_file = 'sha3.nelua',
  includes = {'sha3.h'},
  include_dirs = {'.'},
  include_names = {'^sha3', '^SHA3'},
  output_head = [==[
##[[
cinclude 'sha3.h'
cfile 'sha3.c'
]]
]==]
}

-- miniz
nldecl.generate_bindings_file{
  output_file = 'miniz.nelua',
  defines = {
    'MINIZ_NO_ARCHIVE_APIS',
    'MINIZ_NO_ZLIB_APIS',
    'MINIZ_NO_ZLIB_COMPATIBLE_NAME',
    'MINIZ_NO_STDIO',
    'MINIZ_NO_TIME',
  },
  includes = {'miniz.h'},
  include_dirs = {'.'},
  include_names = {
    '^tdefl_', '^TDEFL_', '^tinfl_', '^TINFL_',
    mz_free = true,
    mz_adler32 = true,
    mz_crc32 = true,
  },
  output_head = [==[
##[[
cdefine 'MINIZ_NO_ARCHIVE_APIS'
cdefine 'MINIZ_NO_ZLIB_APIS'
cdefine 'MINIZ_NO_ZLIB_COMPATIBLE_NAME'
cdefine 'MINIZ_NO_STDIO'
cdefine 'MINIZ_NO_TIME'
cinclude 'miniz.h'
cinclude 'miniz.c'
]]
]==]
}
