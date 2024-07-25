// Chapter 2 - Drawing an image using its color palette

#include <riv.h>

int main() {
    // Load the image color palette starting into drawing color palette at index 32
    riv_load_palette("simple_dungeon_crawler_16x16.png", 32);
    // Load the image from a file
    int img_id = riv_make_image("simple_dungeon_crawler_16x16.png", 255);
    // Main loop
    do {
        // Draw red background
        riv_clear(RIV_COLOR_LIGHTGREEN);
        // Draw the entire image
        riv_draw_image_rect(img_id, 0, 0, 256, 256, 0, 0, 1, 1);
    } while(riv_present());
    return 0;
}
