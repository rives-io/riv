// Chapter 9 - Saving scores

// Header including all RIV APIs
#include <riv.h>

// Constants
enum {
    MAP_SIZE = 16, // 16x16 tiles
    TILE_SIZE = 8, // 8x8 pixels
    GAME_SPRITESHEET = 1,
};

// Game state
bool started; // true when game has started
bool ended; // true when game has ended
int apples; // total amount of apples eaten
int ticks; // total amount of game iterations
int score; // game overall score
riv_vec2i apple_pos; // position of the current apple
riv_vec2i head_pos; // position of the snake head
riv_vec2i head_dir; // direction of the snake head
riv_vec2i tail_pos; // position of the snake tail
riv_vec2i snake_body[MAP_SIZE][MAP_SIZE]; // move directions for the snake body

// Sound effects
riv_waveform_desc start_sfx = {
    .type = RIV_WAVEFORM_PULSE,
    .attack = 0.01f, .decay = 0.01f, .sustain = 0.1f, .release = 0.01f,
    .start_frequency = RIV_NOTE_A3, .end_frequency = RIV_NOTE_A4,
    .amplitude = 0.25f, .sustain_level = 0.5f,
};
riv_waveform_desc end_sfx = {
    .type = RIV_WAVEFORM_PULSE,
    .attack = 0.01f, .decay = 0.01f, .sustain = 0.1f, .release = 0.01f,
    .start_frequency = RIV_NOTE_A3, .end_frequency = RIV_NOTE_A2,
    .amplitude = 0.5f, .sustain_level = 0.5f,
};
riv_waveform_desc eat_sfx = {
    .type = RIV_WAVEFORM_PULSE,
    .attack = 0.01f, .decay = 0.01f, .sustain = 0.1f, .release = 0.01f,
    .start_frequency = RIV_NOTE_A4, .end_frequency = RIV_NOTE_A4,
    .amplitude = 0.25f, .sustain_level = 0.5f,
};

// Check if position collides with snake body
bool collides_with_body(riv_vec2i pos) {
    return !(snake_body[pos.y][pos.x].x == 0 && snake_body[pos.y][pos.x].y == 0);
}

// Spawn apple in a new position
bool respawn_apple() {
    // Returns false when there is not enough space to spawn more apples
    if ((apples + 2) < (MAP_SIZE * MAP_SIZE)) {
        do { // Try to spawn until there is no collision with its body
            apple_pos = (riv_vec2i){riv_rand_uint(MAP_SIZE-1), riv_rand_uint(MAP_SIZE-1)};
        } while (collides_with_body(apple_pos));
        return true;
    }
    return false;
}

// Called when game starts
void start_game() {
    riv_printf("GAME START\n");
    started = true;
    head_dir = (riv_vec2i){0, -1};
    head_pos = (riv_vec2i){MAP_SIZE / 2, MAP_SIZE / 2};
    tail_pos = (riv_vec2i){head_pos.x - head_dir.x, head_pos.y - head_dir.y};
    snake_body[head_pos.y][head_pos.x] = head_dir;
    snake_body[tail_pos.y][tail_pos.x] = head_dir;
    respawn_apple();
    // Play start sound
    riv_waveform(&start_sfx);
}

// Called when game ends
void end_game() {
    riv_printf("GAME OVER\n");
    ended = true;
    // Quit in 3 seconds
    riv->quit_frame = riv->frame + 3*riv->target_fps;
    // Play end sound
    riv_waveform(&end_sfx);
}

// Update game logic
void update_game() {
    ticks++;
    // Set head direction based on inputs
    if (riv->keys[RIV_GAMEPAD_UP].press) {
        head_dir = (riv_vec2i){0, -1};
    } else if (riv->keys[RIV_GAMEPAD_DOWN].press) {
        head_dir = (riv_vec2i){0, 1};
    } else if (riv->keys[RIV_GAMEPAD_LEFT].press) {
        head_dir = (riv_vec2i){-1, 0};
    } else if (riv->keys[RIV_GAMEPAD_RIGHT].press) {
        head_dir = (riv_vec2i){1, 0};
    }
    // Save head move direction
    snake_body[head_pos.y][head_pos.x] = head_dir;
    // Move head
    riv_vec2i next_head_pos = (riv_vec2i){head_pos.x + head_dir.x, head_pos.y + head_dir.y};
    // Check if the head collides with the map boundary
    if (next_head_pos.x < 0 || next_head_pos.y < 0 || next_head_pos.x >= MAP_SIZE || next_head_pos.y >= MAP_SIZE) {
        end_game();
    // Check if the head collides with its body
    } else if (collides_with_body(next_head_pos)) {
        end_game();
    } else {
        // Place the head on the body (will be overwritten later)
        head_pos = next_head_pos;
        snake_body[head_pos.y][head_pos.x] = head_dir;
        // Check if the head collides with the apple
        if (head_pos.x == apple_pos.x && head_pos.y == apple_pos.y) { // Apple was eaten
            apples++;
            riv_printf("APPLES %d\n", apples);
            // Play eat sound
            riv_waveform(&eat_sfx);
            if (!respawn_apple()) { // End game when cannot spawn more apples
                end_game();
            }
        } else { // Apple was not eaten
            // Remove tail
            riv_vec2i tail_dir = snake_body[tail_pos.y][tail_pos.x];
            snake_body[tail_pos.y][tail_pos.x] = (riv_vec2i){0,0};
            // Update tail position
            tail_pos = (riv_vec2i){tail_pos.x + tail_dir.x, tail_pos.y + tail_dir.y};
        }
    }
}

// Draw the game map
void draw_game() {
    // Draw apple
    riv_draw_sprite(0, GAME_SPRITESHEET, apple_pos.x*TILE_SIZE, apple_pos.y*TILE_SIZE, 1, 1, 1, 1);
    // Draw snake body
    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            if (collides_with_body((riv_vec2i){x,y})) {
                riv_draw_rect_fill(x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE, RIV_COLOR_LIGHTGREEN);
            }
        }
    }
    // Draw snake head
    int spr_id = 2 + ((head_dir.x != 0) ? 1 : 0);
    int flip_x = (head_dir.x == -1) ? -1 : 1;
    int flip_y = (head_dir.y == -1) ? -1 : 1;
    riv_draw_sprite(spr_id, GAME_SPRITESHEET, head_pos.x*TILE_SIZE, head_pos.y*TILE_SIZE, 1, 1, flip_x, flip_y);
    // Draw score
    char buf[128];
    riv_snprintf(buf, sizeof(buf), "SCORE %d", score);
    riv_draw_text(buf, RIV_SPRITESHEET_FONT_3X5, RIV_BOTTOMLEFT, 1, 128-1, 1, RIV_COLOR_WHITE);
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

// Output scores on the output card
void update_scores() {
    score = apples * MAP_SIZE * 2 - ticks;
    riv->outcard_len = riv_snprintf((char*)riv->outcard, RIV_SIZE_OUTCARD, "JSON{\"score\":%d,\"apples\":%d,\"ticks\":%d}", score, apples, ticks);
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
    // Output scores
    update_scores();
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
    // Load sprites
    riv_make_spritesheet(riv_make_image("sprites.png", 0), 8, 8);
    // Main loop, keep presenting frames until user quit or game ends
    do {
        // Update game state
        update();
        // Draw game graphics
        draw();
    } while(riv_present());
    return 0;
}
