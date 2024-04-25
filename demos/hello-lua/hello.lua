#!/usr/bin/lua5.4
local ffi = require("cffi")
ffi.cdef[[
typedef enum riv_color_id {
  RIV_COLOR_BLACK       = 0,
  RIV_COLOR_WHITE       = 1,
} riv_color_id;
typedef enum riv_spritesheet_id {
  RIV_SPRITESHEET_FONT_5X7 = 4095,
} riv_spritesheet_id;
typedef enum riv_align {
  RIV_CENTER = 4,
} riv_align;
typedef struct riv_vec2i {int64_t x; int64_t y;} riv_vec2i;
bool riv_present();
void riv_clear(uint32_t col);
riv_vec2i riv_draw_text(const char* text, uint64_t sps_id, riv_align anchor, int64_t x, int64_t y, int64_t size, int64_t col);
]]
local L = ffi.load("riv")

repeat
  L.riv_clear(L.RIV_COLOR_BLACK)
  L.riv_draw_text("hello world!", L.RIV_SPRITESHEET_FONT_5X7, L.RIV_CENTER, 128, 128, 2, L.RIV_COLOR_WHITE)
until not L.riv_present()
