#include <riv.h>

int main() {
    do {
        // clear screen
        riv_clear(RIV_COLOR_BLACK);
        // draw hello world
        riv_draw_text("hello world!", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, 128, 128, 2, RIV_COLOR_WHITE);
    } while(riv_present());
}
