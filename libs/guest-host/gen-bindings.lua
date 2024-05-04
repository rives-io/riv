local nldecl = require 'nelua.plugins.nldecl'

-- sha256
nldecl.generate_bindings_file{
  output_file = 'sha256.nelua',
  includes = {'sha256.h'},
  include_dirs = {'.'},
  include_names = {'^sha256', '^sha256'},
  output_head = [==[
##[[
cinclude 'sha256.h'
cinclude 'sha256.c'
]]
]==]
}

-- miniz
nldecl.generate_bindings_file{
  output_file = 'miniz.nelua',
  defines = {
    'MINIZ_NO_ARCHIVE_APIS',
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
cdefine 'MINIZ_NO_STDIO'
cdefine 'MINIZ_NO_TIME'
cdefine 'MINIZ_EXPORT static'
cinclude 'miniz.h'
cinclude 'miniz.c'
]]
]==]
}

-- spng
nldecl.generate_bindings_file{
  output_file = 'spng.nelua',
  includes = {'spng.h'},
  include_dirs = {'.'},
  include_names = {'^spng_', '^SPNG_'},
  output_head = [==[
##[[
cdefine 'SPNG_USE_MINIZ'
cdefine 'SPNG_DISABLE_OPT'
cdefine 'SPNG_API static'
cinclude 'spng.h'
cinclude 'spng.c'
]]
]==]
}

-- emscripten
nldecl.generate_bindings_file{
  output_file = 'emscripten.nelua',
  includes = {
    '<emscripten/emscripten.h>',
    '<emscripten/html5.h>'
  },
  cc = 'emcc',
  -- include_dirs = {'.'},
  include_names = {'^em_', '^EM_', '^emscripten_', '^EMSCRIPTEN_'},
  output_head = [==[
##[[
cinclude '<emscripten/emscripten.h>'
cinclude '<emscripten/html5.h>'
]]
]==]
}
