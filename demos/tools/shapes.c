//riv-jit-c (DO NOT REMOVE THIS LINE)
#include <riv.h>
#include <math.h>

int main() {
  int img_id = riv_make_image("simple_dungeon_crawler_16x16.png", RIV_COLOR_BLACK);
  riv->target_fps = 30;
  do {
    riv_clear(RIV_COLOR_BLACK);
    float t = riv->time;
    for (int i=0;i<16;++i) {
      int s = 48;
      int ox = (i % 4)*64 + 32;
      int oy = (i / 4)*64 + 32;
      int dx = s + sinf(t + i)*s/4;
      int dy = s/2 + sinf(2*t + i)*s/4;
      float rot = t + i;
      char *name = "";
      switch(i) {
        case 0: name="RECT FILL"; riv_draw_rect_fill(ox-dx/2, oy-dy/2, dx, dy, RIV_COLOR_BLUE); break;
        case 1: name="RECT LINE"; riv_draw_rect_line(ox-dx/2, oy-dy/2, dx, dy, RIV_COLOR_LIGHTBLUE); break;
        case 2: name="CIRCLE FILL"; riv_draw_circle_fill(ox, oy, dx, RIV_COLOR_GREEN); break;
        case 3: name="CIRCLE LINE"; riv_draw_circle_line(ox, oy, dx, RIV_COLOR_LIGHTGREEN); break;
        case 4: name="BOX FILL"; riv_draw_box_fill(ox, oy, dx, dy, rot, RIV_COLOR_YELLOW); break;
        case 5: name="BOX LINE"; riv_draw_box_line(ox, oy, dx, dy, rot, RIV_COLOR_LIGHTYELLOW); break;
        case 6: name="ELLIPSE FILL"; riv_draw_ellipse_fill(ox, oy, dx, dy, RIV_COLOR_RED); break;
        case 7: name="ELLIPSE LINE"; riv_draw_ellipse_line(ox, oy, dx, dy, RIV_COLOR_LIGHTRED); break;
        case 8: name="QUAD FILL"; riv_draw_quad_fill(ox-dy/2, oy-dy/2, ox+dy/2, oy-dy/2, ox-dx/2, oy+dy/2, ox+dx/2, oy+dy/2, RIV_COLOR_PINK); break;
        case 9: name="QUAD LINE"; riv_draw_quad_line(ox-dy/2, oy-dy/2, ox+dy/2, oy-dy/2, ox-dx/2, oy+dy/2, ox+dx/2, oy+dy/2, RIV_COLOR_LIGHTPINK); break;
        case 10: name="TRIANGLE FILL"; riv_draw_triangle_fill(ox-dy/2, oy-dy/2, ox-dx/2, oy+dy/2, ox+dx/2, oy+dy/2, RIV_COLOR_PEACH); break;
        case 11: name="TRIANGLE LINE"; riv_draw_triangle_fill(ox-dy/2, oy-dy/2, ox-dx/2, oy+dy/2, ox+dx/2, oy+dy/2, RIV_COLOR_LIGHTPEACH); break;
        case 12: name="POINT"; riv_draw_point(ox+dx-s, oy, RIV_COLOR_LIGHTRED); break;
        case 13: name="IMAGE RECT"; riv_draw_image_rect(img_id, ox-16, oy-16, 16, 16, ((riv->frame / 4) % 6) * 16, 128, 2, 2); break;
        case 14: name="TEXT"; riv_draw_text("HELLO", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, ox, oy, (int)(sinf(i+t*4)+2), RIV_COLOR_WHITE); break;
        case 15: name="TEXT EX"; riv_draw_text_ex("HELLO", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, ox, oy, (int)(sinf(i+t*3)+2), (int)(sinf(i+t*4)+2), 2, 2, RIV_COLOR_WHITE); break;
      }
      riv_draw_text(name, RIV_SPRITESHEET_FONT_3X5, RIV_CENTER, ox, oy-24, 1, RIV_COLOR_LIGHTGREY);
    }
  }
  while(riv_present());
  return 0;
}
