#!/usr/bin/qjs --unhandled-rejection
import { CFunction } from '/usr/lib/qjs/quickjs-ffi.js';
import * as ffi from '/usr/lib/qjs/quickjs-ffi.so';

const RIV_COLOR_BLACK = 0;
const RIV_COLOR_WHITE = 1;
const RIV_SPRITESHEET_FONT_5X7 = 4095;
const RIV_CENTER = 4;

const riv_present = (new CFunction('libriv.so', 'riv_present', null, 'sint8')).invoke;
const riv_clear = (new CFunction('libriv.so', 'riv_clear', null, 'void', 'sint64')).invoke;
const riv_draw_text = (new CFunction('libriv.so', 'riv_draw_text', null, 'void', 'uint64', 'string', 'uint64', 'sint32', 'sint64', 'sint64', 'sint64', 'sint64')).invoke;
const riv_memalloc = (new CFunction('libriv.so', 'riv_memalloc', null, 'uint64', 'uint64')).invoke;
let riv_draw_text_ret = riv_memalloc(32);

do {
  riv_clear(RIV_COLOR_BLACK);
  riv_draw_text(riv_draw_text_ret, "hello world!", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, 128, 128, 2, RIV_COLOR_WHITE);
} while(riv_present());
