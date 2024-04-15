#!/usr/bin/luajit
local ffi = require("ffi")
ffi.cdef[[
typedef enum riv_color_id {
  RIV_COLOR_WHITE       = 1,
} riv_color_id;
typedef enum riv_spritesheet_id {
  RIV_SPRITESHEET_FONT_5X7 = 1023,
} riv_spritesheet_id;
typedef struct riv_vec2i {int64_t x; int64_t y;} riv_vec2i;
bool riv_present();
void riv_clear_screen(uint32_t col);
riv_vec2i riv_draw_text(const char* text, uint64_t sps_id, int64_t x0, int64_t y0, int64_t col, int64_t mw, int64_t mh, int64_t sx, int64_t sy);
]]
local L = ffi.load("riv")

repeat
  L.riv_clear_screen(4)
  L.riv_draw_text("HELLO FROM LUA JIT!", L.RIV_SPRITESHEET_FONT_5X7, 32, 32, L.RIV_COLOR_WHITE, 1, 1, 1, 1)
until not L.riv_present()