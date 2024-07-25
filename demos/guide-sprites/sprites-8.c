// Chapter 8 - Making a simple tiled game

#include <riv.h>

enum { // Constants
    SCREEN_SIZE = 256,
    TILE_SIZE = 16,
    MAP_SIZE = 16,
    MAP_LAYERS = 2,
    MAP_LAYER_GROUND = 0,
    MAP_LAYER_ITEMS = 1,
    SPRITESHEET_GAME = 1,
    SPR_COINS = 1,
    SPR_PLAYER_IDLE = 128,
    SPR_PLAYER_WALK = 144,
    NUM_FRAMES_PLAYER = 6,
};

// Map
static uint16_t map[MAP_LAYERS][MAP_SIZE][MAP_SIZE] = {
    { // layer 0 - grounds
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        {  0,  9, 27,  9, 27,  9, 27,  9, 27,  9, 27,  9, 27,  9, 27,  0},
        {  0, 53, 36, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37,  0},
        {  0, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37, 53, 50, 53, 37,  0},
        {  0, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37,  0},
        {  0, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37,  0},
        {  0, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37,  0},
        {  0, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37,  0},
        {  0, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37,  0},
        {  0, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37,  0},
        {  0, 53, 37, 53, 37, 53, 37, 52, 37, 53, 37, 53, 37, 53, 37,  0},
        {  0, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37, 53, 35, 53, 37,  0},
        {  0, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37,  0},
        {  0, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37,  0},
        {  0, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37,  0},
        {  0, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37, 53, 37,  0},
    },
    { // layer 1 - objects
        { 14, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 13},
        { 30,150,  0,  4,  0,  0,  0,  4,  0,  0,  0,  0,  3,  0,150, 29},
        { 30,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 29},
        { 30,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 29},
        { 30,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 29},
        { 30,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 29},
        { 30,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0, 29},
        { 30,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 29},
        { 30,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 29},
        { 30,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 29},
        { 30,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 29},
        { 30,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0, 29},
        { 30,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0, 29},
        { 30,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 29},
        { 30,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0, 29},
        { 46, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 45},
    },
};

static riv_vec2i player_pos = {(SCREEN_SIZE-TILE_SIZE)/2, (SCREEN_SIZE-TILE_SIZE)/2};
static int64_t player_xflip = 1;
static int64_t player_speed = 1;
static bool player_walking = false;
static riv_recti player_spr_bbox;
static riv_recti coins_spr_bbox;

int64_t clamp(int64_t v, int64_t min, int64_t max) {v = v < min ? min : v; v = v > max ? max : v; return v; }

// Checks whether a rectangle overlaps with another rectangle
bool overlaps_recti(riv_recti a, riv_recti b) { return a.x + a.width > b.x && b.x + b.width > a.x && a.y + a.height > b.y && b.y + b.height > a.y; }

void update() {
    // Move player
    if (riv->keys[RIV_GAMEPAD_RIGHT].down) {
        player_pos.x += player_speed;
        player_xflip = 1; // Face right
    }
    if (riv->keys[RIV_GAMEPAD_LEFT].down) {
        player_pos.x -= player_speed;
        player_xflip = -1; // Face left
    }
    if (riv->keys[RIV_GAMEPAD_DOWN].down) {
        player_pos.y += player_speed;
    }
    if (riv->keys[RIV_GAMEPAD_UP].down) {
        player_pos.y -= player_speed;
    }
    // Limit player position to map boundary
    player_pos.x = clamp(player_pos.x, TILE_SIZE, SCREEN_SIZE-2*TILE_SIZE);
    player_pos.y = clamp(player_pos.y, TILE_SIZE*3/2, SCREEN_SIZE-TILE_SIZE-TILE_SIZE/2);
    // Update player walking state
    player_walking = (riv->keys[RIV_GAMEPAD_RIGHT].down || riv->keys[RIV_GAMEPAD_LEFT].down ||
                      riv->keys[RIV_GAMEPAD_DOWN].down || riv->keys[RIV_GAMEPAD_UP].down);
    // Compute player bounding box
    riv_recti player_bbox = player_spr_bbox;
    player_bbox.x += player_pos.x;
    player_bbox.y += player_pos.y;
    // Check if it collides with a coin
    for (int y=0;y<MAP_SIZE;++y) { // For each map row
        for (int x=0;x<MAP_SIZE;++x) { // For each map column
            if (map[1][y][x] == SPR_COINS) { // Map tile has a coins
                // Compute coins bounding box
                riv_recti coins_bbox = coins_spr_bbox;
                coins_bbox.x += x*TILE_SIZE;
                coins_bbox.y += y*TILE_SIZE;
                if (overlaps_recti(player_bbox, coins_bbox)) { // Coins overlap with player
                    // Remove coins
                    map[1][y][x] = 0;
                    // Play coins sounds
                    riv_waveform(&(riv_waveform_desc){
                        .type = RIV_WAVEFORM_TRIANGLE,
                        .attack = 0.025f, .decay = 0.075f, .sustain = 0.075f, .release = 0.075f,
                        .start_frequency = 1760.0f,
                        .sustain_level = 0.1,
                    });
                }
            }
        }
    }
}

void draw() {
    // Draw map
    for (int l=0;l<MAP_LAYERS;++l) { // For each map layer
        riv->draw.color_key_disabled = l == 0; // Disable transparency for ground (optimization)
        for (int y=0;y<MAP_SIZE;++y) { // For each map row
            for (int x=0;x<MAP_SIZE;++x) { // For each map column
                int spr = map[l][y][x];
                if (spr != 0) { // Draw tile sprite
                    riv_draw_sprite(spr, SPRITESHEET_GAME, x*TILE_SIZE, y*TILE_SIZE, 1, 1, 1, 1);
                }
            }
        }
    }
    // Compute player sprite id based on walking state
    uint32_t id = (player_walking ? SPR_PLAYER_WALK : SPR_PLAYER_IDLE) + (riv->frame / 4) % NUM_FRAMES_PLAYER;
    // Draw player sprite
    riv_draw_sprite(id, SPRITESHEET_GAME, player_pos.x, player_pos.y, 1, 1, player_xflip, 1);
}

int main() {
    // Load the image color palette starting into drawing color palette at index 32
    riv_load_palette("simple_dungeon_crawler_16x16.png", 32);
    riv_make_spritesheet(riv_make_image("simple_dungeon_crawler_16x16.png", 0xff), TILE_SIZE, TILE_SIZE);
    // Compute bounding box for collision checking
    player_spr_bbox = riv_get_sprite_bbox(SPR_PLAYER_IDLE, SPRITESHEET_GAME, 1, 1);
    coins_spr_bbox = riv_get_sprite_bbox(SPR_COINS, SPRITESHEET_GAME, 1, 1);
    // Main loop
    do {
        update();
        draw();
    } while(riv_present());
    return 0;
}
