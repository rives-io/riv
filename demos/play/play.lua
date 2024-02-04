
local function read_indexed_ppm(filename)
  local file, open_err = io.open(filename,'rb')
  if not file then return nil, string.format('file %s: %s', filename, open_err) end
  local filedata,read_err = file:read('*a')
  file:close()
  if not filedata then return nil, read_err end
  if not filedata then return string.format('file %s: %s', filename, read_err) end
  local ppm_patt = require'nelua.thirdparty.lpegrex'.compile([[
  file <- {| 'P6' skip
             {:width: %d+ :} skip
             {:height: %d+ :} skip
             {:maxval: %d+ :} %s
             {:data: .* :} |}
  skip <- (%s / '#' (!%nl .)* %nl)+
  ]])
  local ppm = ppm_patt:match(filedata)
  if not ppm then return nil, string.format('file %s: unsupported PPM file format', filename) end
  ppm.width, ppm.height, ppm.maxval = tonumber(ppm.width), tonumber(ppm.height), tonumber(ppm.maxval)
  assert(ppm.maxval == 255)
  local pos = 1
  local data = ppm.data
  local size = ppm.height*ppm.width
  local usedcolors = {[0] = true}
  local pixels = {}
  local colors = {}
  for i=1,size do
    local color
    color, pos = string.unpack('>I3', data, pos)
    pixels[i] = color
    if not usedcolors[color] then
      colors[#colors+1] = color
      usedcolors[color] = true
    end
  end
  table.sort(colors)
  local index2color = {[0]=0}
  local color2index = {[0]=0}
  for i,color in ipairs(colors) do
    index2color[i] = color
    color2index[color] = i
  end
  local indexdata = {}
  for i=1,size do
    indexdata[i] = string.char(color2index[pixels[i]])
  end
  return indexdata, ppm.width, ppm.height, index2color
end

print 'go'
-- local f = "demos/play/font.ppm"
local f = "/home/bart/f.ppm"
local t,w,h,index2color = assert(read_indexed_ppm(f))
local gw = 7
local pitch = 7*16
for i=0,14-1 do
  for y=0,h-1 do
    for x=i*pitch,(i+1)*pitch-1 do
      -- io.write(index2color[string.byte(t[y*w + x + 1])] == 0xffffff and 'X' or ' ')
      if x ~= i*pitch and (x % gw == 0 or x % gw == 6) then
        io.write('  ')
      else
        io.write(string.byte(t[y*w + x + 1]) == 2 and '#' or '.')
      end
    end
    io.write('\n')
  end
  io.write('\n')
  io.write('\n')
end
os.exit(0)