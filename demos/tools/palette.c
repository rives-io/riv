//riv-jit-c (DO NOT REMOVE THIS LINE)
#include <riv.h>

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
    char s[32];
    riv_draw_rect_fill(x, y, w, h, i);
    riv_draw_text(({riv_snprintf(s, sizeof(s), "%d", i); s;}), RIV_SPRITESHEET_FONT_5X7, RIV_LEFT, x + b, y + h / 2, 1, col);
    riv_draw_text(({riv_snprintf(s, sizeof(s), "#%06X", rgb); s;}), RIV_SPRITESHEET_FONT_5X7, RIV_RIGHT, x+w-b, y + h / 2, 1, col);
  }
  do {} while(riv_present());
  return 0;
}
