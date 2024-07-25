// Chapter 6 - Drawing a sprite bounding box

#include <riv.h>

int main() {
    // Load the image color palette starting into drawing color palette at index 32
    riv_load_palette("simple_dungeon_crawler_16x16.png", 32);
    // Load the image from a file
    int img_id = riv_make_image("simple_dungeon_crawler_16x16.png", 255);
    // Create a sprite sheet from an image
    int sps_id = riv_make_spritesheet(img_id, 16, 16);
    // Define player bounding box
    riv_recti bbox = riv_get_sprite_bbox(128, sps_id, 1, 1);
    // Main loop
    do {
        // Draw red background
        riv_clear(RIV_COLOR_BLACK);
        // Draw the standard sprite bounding box
        riv_draw_rect_fill(120, 120, 16, 16, RIV_COLOR_LIGHTGREEN);
        // Draw the player sprite
        riv_draw_sprite(128, sps_id, 120, 120, 1, 1, 1, 1);
        // Draw the sprite bounding box
        riv_draw_rect_line(120 + bbox.x, 120 + bbox.y, bbox.width, bbox.height, RIV_COLOR_WHITE);
    } while(riv_present());
    return 0;
}
