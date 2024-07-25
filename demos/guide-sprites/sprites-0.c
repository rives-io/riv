// Chapter 0 - Tool to draw sprite numbers

#include <riv.h>

int main() {
    // Load the image color palette starting into drawing color palette at index 32
    riv_load_palette("simple_dungeon_crawler_16x16.png", 32);
    // Load the image from a file
    int img_id = riv_make_image("simple_dungeon_crawler_16x16.png", 255);
    // Main loop
    do {
        // Draw red background
        for (int y=0;y<16;++y) {
            for (int x=0;x<16;++x) {
                int n = y*16 + x;
                riv_draw_rect_fill(x*16, y*16, 16, 16, (x+y) % 2 == 0 ? RIV_COLOR_GREY : RIV_COLOR_LIGHTGREY);
            }
        }
        // Draw the entire image
        riv_draw_image_rect(img_id, 0, 0, 256, 256, 0, 0, 1, 1);
        // Draw numbers
        for (int y=0;y<16;++y) {
            for (int x=0;x<16;++x) {
                int n = y*16 + x;
                riv_draw_text(riv_tprintf("%d",n), RIV_SPRITESHEET_FONT_3X5, RIV_TOPLEFT, x*16+1, y*16+1, 1, RIV_COLOR_WHITE);
            }
        }
    } while(riv_present());
    return 0;
}
