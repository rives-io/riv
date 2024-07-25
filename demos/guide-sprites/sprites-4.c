// Chapter 4 - Drawing a sprite

#include <riv.h>

int main() {
    // Load the image color palette starting into drawing color palette at index 32
    riv_load_palette("simple_dungeon_crawler_16x16.png", 32);
    // Load the image from a file
    int img_id = riv_make_image("simple_dungeon_crawler_16x16.png", 255);
    // Create a sprite sheet of 16x16 sprites from an image
    int sps_id = riv_make_spritesheet(img_id, 16, 16);
    // Main loop
    do {
        // Draw red background
        riv_clear(RIV_COLOR_LIGHTGREEN);
        // Draw the knight sprite
        riv_draw_sprite(128, sps_id, 112, 112, 1, 1, -2, 2);
    } while(riv_present());
    return 0;
}