// Chapter 7 - Drawing tiled maps

#include <riv.h>

enum {
    TILE_SIZE = 16,
    MAP_SIZE = 16,
    MAP_LAYERS = 2,
};

// The tile map
uint16_t map[MAP_LAYERS][MAP_SIZE][MAP_SIZE] = {
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

int main() {
    // Load the image color palette starting into drawing color palette at index 32
    riv_load_palette("simple_dungeon_crawler_16x16.png", 32);
    // Load the image from a file
    int img_id = riv_make_image("simple_dungeon_crawler_16x16.png", 255);
    // Create a sprite sheet from an image
    int sps_id = riv_make_spritesheet(img_id, TILE_SIZE, TILE_SIZE);
    // Main loop
    do {
        // Draw black background
        riv_clear(RIV_COLOR_BLACK);
        // Draw map
        for (int l=0;l<MAP_LAYERS;++l) { // For each layer
            for (int y=0;y<MAP_SIZE;++y) { // For each row
                for (int x=0;x<MAP_SIZE;++x) { // For each column
                    int spr = map[l][y][x];
                    if (spr != 0) { // Draw tile sprite
                        riv_draw_sprite(spr, sps_id, x*TILE_SIZE, y*TILE_SIZE, 1, 1, 1, 1);
                    }
                }
            }
        }
    } while(riv_present());
    return 0;
}
