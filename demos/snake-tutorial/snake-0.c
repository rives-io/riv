// Header including all RIV APIs
#include <riv.h>

// Called every frame to draw the game
void draw() {
    // Clear screen
    riv_clear(RIV_COLOR_DARKSLATE);
    // Draw snake title
    riv_draw_text(
        "snake",                  // text to draw
        RIV_SPRITESHEET_FONT_5X7, // sprite sheet id of the font
        RIV_CENTER,               // anchor point on the text bounding box
        128,                      // anchor x
        128,                      // anchor y
        4,                        // text size multiplier
        RIV_COLOR_LIGHTGREEN      // text color
    );
}

// Entry point
int main() {
    // Main loop, keep presenting frames until user quit or game ends
    do {
        // Draw game graphics
        draw();
    } while(riv_present());
    return 0;
}
