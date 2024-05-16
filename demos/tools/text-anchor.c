#include <riv.h>

const char *get_align_name(riv_align align) {
  switch(align) {
  case RIV_TOPLEFT: return "TOPLEFT";
  case RIV_TOP: return "TOP";
  case RIV_TOPRIGHT: return "TOPRIGHT";
  case RIV_LEFT: return "LEFT";
  case RIV_CENTER: return "CENTER";
  case RIV_RIGHT: return "RIGHT";
  case RIV_BOTTOMLEFT: return "BOTTOMLEFT";
  case RIV_BOTTOM: return "BOTTOM";
  case RIV_BOTTOMRIGHT: return "BOTTOMRIGHT";
  }
}

int main() {
  riv->target_fps = 1;
  do {
    riv_clear(RIV_COLOR_BLACK);
    int x = 128, y = 128;
    int i = riv->frame % 9;
    riv_draw_box_fill(x, y, 128, 64, 0, RIV_COLOR_GREY);
    riv_draw_text(get_align_name(i),
      RIV_SPRITESHEET_FONT_5X7,
      RIV_CENTER, x, y-64, 1, RIV_COLOR_WHITE);
    riv_recti bbox = riv_draw_text("TEXT",
      RIV_SPRITESHEET_FONT_5X7,
      i, x, y, 2, RIV_COLOR_YELLOW);
    riv_draw_rect_line(bbox.x-1, bbox.y-1, bbox.width+2, bbox.height+2, RIV_COLOR_LIGHTGREEN);
    riv_draw_rect_fill(x-1, y-1, 3, 3, RIV_COLOR_LIGHTRED);
  } while(riv_present());
  return 0;
}
