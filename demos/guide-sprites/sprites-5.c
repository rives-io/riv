// Chapter 5 - Drawing an animated sprite

#include <riv.h>

int main() {
    // Load the image color palette starting into drawing color palette at index 32
    riv_load_palette("simple_dungeon_crawler_16x16.png", 32);
    // Load the image from a file
    int img_id = riv_make_image("simple_dungeon_crawler_16x16.png", 255);
    // Create a sprite sheet from an image
    int sps_id = riv_make_spritesheet(img_id, 16, 16);
    // Player direction
    int dir = 1;
    // Main loop
    do {
        // Change player direction
        if (riv->keys[RIV_GAMEPAD_LEFT].press) dir = -1;
        if (riv->keys[RIV_GAMEPAD_RIGHT].press) dir = 1;
        // Draw red background
        riv_clear(RIV_COLOR_LIGHTGREEN);
        // Compute animation frame
        int anim = (riv->frame / 4) % 6;
        // Draw the player sprite animated
        riv_draw_sprite(128 + anim, sps_id, 112, 112, 1, 1, 2*dir, 2);
    } while(riv_present());
    return 0;
}
