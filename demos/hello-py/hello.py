#!/usr/bin/micropython
import ffi

RIV_COLOR_BLACK = 0
RIV_COLOR_WHITE = 1
RIV_SPRITESHEET_FONT_5X7 = 4095
RIV_CENTER = 4

libriv = ffi.open("libriv.so")
riv_present = libriv.func("i", "riv_present", "")
riv_clear = libriv.func("v", "riv_clear", "l")
riv_draw_text = libriv.func("v", "riv_draw_text", "sLillll")

while True:
  riv_clear(RIV_COLOR_BLACK)
  riv_draw_text("hello world!", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, 128, 128, 2, RIV_COLOR_WHITE)
  if not riv_present():
    break
