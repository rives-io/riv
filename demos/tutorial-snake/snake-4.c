// Chapter 4 - Spawning the apple

// Header including all RIV APIs
#include <riv.h>

// Constants
enum {
    MAP_SIZE = 16, // 16x16 tiles
    TILE_SIZE = 8, // 8x8 pixels
};

// Game state
bool started; // true when game has started
bool ended; // true when game has ended
riv_vec2i apple_pos; // position of the current apple

// Spawn apple in a new position
bool respawn_apple() {
    apple_pos = (riv_vec2i){riv_rand_uint(MAP_SIZE-1), riv_rand_uint(MAP_SIZE-1)};
    return true;
}

// Called when game starts
void start_game() {
    riv_printf("GAME START\n");
    started = true;
    respawn_apple();
}

// Called when game ends
void end_game() {
    riv_printf("GAME OVER\n");
    ended = true;
    // Quit in 3 seconds
    riv->quit_frame = riv->frame + 3*riv->target_fps;
}

// Update game logic
void update_game() {
    // TODO: update game
    end_game();
}

// Draw the game map
void draw_game() {
    // Draw apple
    riv_draw_rect_fill(apple_pos.x*TILE_SIZE, apple_pos.y*TILE_SIZE, TILE_SIZE, TILE_SIZE, RIV_COLOR_LIGHTRED);
}

// Draw game start screen
void draw_start_screen() {
    // Draw snake title
    riv_draw_text(
        "snake",                  // text to draw
        RIV_SPRITESHEET_FONT_5X7, // sprite sheet id of the font
        RIV_CENTER,               // anchor point on the text bounding box
        64,                       // anchor x
        64,                       // anchor y
        2,                        // text size multiplier
        RIV_COLOR_LIGHTGREEN      // text color
    );
    // Make "press to start blink" by changing the color depending on the frame number
    uint32_t col = (riv->frame % 2 == 0) ? RIV_COLOR_LIGHTRED : RIV_COLOR_DARKRED;
    // Draw press to start
    riv_draw_text("PRESS TO START", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, 64, 64+16, 1, col);
}

// Draw game over screen
void draw_end_screen() {
    // Draw last game frame
    draw_game();
    // Draw GAME OVER
    riv_draw_text("GAME OVER", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, 64, 64, 2, RIV_COLOR_RED);
}

// Called every frame to update game state
void update() {
    if (!started) { // Game not started yet
        // Let game start whenever a key has been pressed
        if (riv->key_toggle_count > 0) {
            start_game();
        }
    } else if (!ended) { // Game is progressing
        update_game();
    }
}

// Called every frame to draw the game
void draw() {
    // Clear screen
    riv_clear(RIV_COLOR_DARKSLATE);
    // Draw different screens depending on the game state
    if (!started) { // Game not started yet
        draw_start_screen();
    } else if (!ended) { // Game is progressing
        draw_game();
    } else { // Game ended
        draw_end_screen();
    }
}

// Entry point
int main() {
    // Set screen size and default frame rate
    riv->width = 128;
    riv->height = 128;
    riv->target_fps = 8;
    // Main loop, keep presenting frames until user quit or game ends
    do {
        // Update game state
        update();
        // Draw game graphics
        draw();
    } while(riv_present());
    return 0;
}
