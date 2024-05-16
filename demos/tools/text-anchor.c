#include <riv.h>
#include <string.h>

int main() {
  // main loop
  riv->target_fps = 1;
  do {
    riv_clear(RIV_COLOR_BLACK);
    riv_recti bbox = riv_draw_text("SOME TEXT",
      RIV_SPRITESHEET_FONT_5X7,
      riv->frame % 9, 128, 128, 2, RIV_COLOR_YELLOW);
    riv_draw_rect_line(bbox.x-1, bbox.y-1, bbox.width+2, bbox.height+2, RIV_COLOR_LIGHTGREY);
    riv_draw_rect_fill(128-1, 128-1, 3, 3, RIV_COLOR_LIGHTRED);
  } while(riv_present());
  return 0;
}
