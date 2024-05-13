#include <riv.h>

uint32_t bswap32(uint32_t val) {
  val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF );
  return (val << 16) | ((val >> 16) & 0xFFFF);
}

int main() {
  for(int i = 0; i < 32; i += 1) {
    int w = 128;
    int h = 16;
    int b = 4;
    int x = (i / 16) * 128;
    int y = (i % 16) * h;
    int rgb = riv->palette[i];
    bool dark = (((rgb  >> 0) & 0xff) + ((rgb  >> 8) & 0xff) + ((rgb  >> 16) & 0xff)) < 0xff;
    int col = dark ? RIV_COLOR_WHITE : RIV_COLOR_BLACK;
    riv_draw_rect_fill(x, y, w, h, i);
    riv_draw_text(riv_tprintf("%d", i), RIV_SPRITESHEET_FONT_5X7, RIV_LEFT, x + b, y + h / 2, 1, col);
    riv_draw_text(riv_tprintf("#%06x", bswap32(rgb) >> 8), RIV_SPRITESHEET_FONT_5X7, RIV_RIGHT, x+w-b, y + h / 2, 1, col);
  }
  do {} while(riv_present());
  return 0;
}
