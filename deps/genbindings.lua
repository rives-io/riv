local nldecl = require 'nelua.plugins.nldecl'

-- nldecl.generate_bindings_file{
--   output_file = 'linux.nelua',
--   parse_includes = {
--     '<sys/mman.h>',
--   },
--   cc = 'gcc',
-- }

nldecl.generate_bindings_file{
  output_file = 'cartesi.nelua',
  parse_includes = {
    '<cartesi.h>',
  },
  include_dirs = {
    '.'
  },
  include_names = {
    '^cm',
    '^CM',
  },
  cc = 'gcc',
}
