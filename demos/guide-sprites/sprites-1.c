// Chapter 1 - Drawing an image

#include <riv.h>

int main() {
    // Load the image from a file
    int img_id = riv_make_image("simple_dungeon_crawler_16x16.png", 255);
    // Main loop
    do {
        // Draw red background
        riv_clear(RIV_COLOR_LIGHTGREEN);
        // Draw the entire image
        riv_draw_image_rect(img_id,
            0, 0,     // screen coordinates x, y
            256, 256, // image source width, height
            0, 0,     // image source coordinates x, y
            1, 1      // multiply width, height
        );
    } while(riv_present());
    return 0;
}
