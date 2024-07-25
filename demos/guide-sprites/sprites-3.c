// Chapter 3 - Drawing an image section

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
        // Draw the a section of the image
        riv_draw_image_rect(img_id,
            112, 112, // screen coordinates x, y
            16, 16,   // image source width, height
            0, 128,   // image source coordinates x, y
            -2, 2     // multiply width, height
        );
    } while(riv_present());
    return 0;
}
