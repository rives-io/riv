#include <riv.h>
#include <math.h>

enum {
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

static uint16_t map[MAP_LAYERS][MAP_SIZE][MAP_SIZE] = {
  { // layer 0 - grounds
    {0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35},
    {0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35},
    {0x25,0x35,0x24,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35},
    {0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x32,0x35,0x25,0x35},
    {0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35},
    {0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35},
    {0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35},
    {0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35},
    {0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35},
    {0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35},
    {0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x34,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35},
    {0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x23,0x35,0x25,0x35},
    {0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35},
    {0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35},
    {0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35},
    {0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35,0x25,0x35},
  },
  { // layer 1 - objects
    {0x2b,0x2c,0x2c,0x2c,0x2c,0x2c,0x2c,0x2c,0x2c,0x2c,0x2c,0x2c,0x2c,0x2c,0x2c,0x2f},
    {0x1d,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x1e},
    {0x1d,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1e},
    {0x1d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1e},
    {0x1d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1e},
    {0x1d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1e},
    {0x1d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x1e},
    {0x1d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1e},
    {0x1d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1e},
    {0x1d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1e},
    {0x1d,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1e},
    {0x1d,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x1e},
    {0x1d,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x1e},
    {0x1d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1e},
    {0x1d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x1e},
    {0x3b,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3f},
  },
};

static riv_vec2i player_pos = {(SCREEN_SIZE-TILE_SIZE)/2, (SCREEN_SIZE-TILE_SIZE)/2};
static int64_t player_xflip = 1;
static int64_t player_speed = 2;
static bool player_walking = false;

int64_t clamp(int64_t v, int64_t min, int64_t max) { v = v < min ? min : v; v = v > max ? max : v; return v; }

void update() {
  if (riv->keys[RIV_GAMEPAD_RIGHT].down) {
    player_pos.x += player_speed;
    player_xflip = 1;
  }
  if (riv->keys[RIV_GAMEPAD_LEFT].down) {
    player_pos.x -= player_speed;
    player_xflip = -1;
  }
  if (riv->keys[RIV_GAMEPAD_DOWN].down) {
    player_pos.y += player_speed;
  }
  if (riv->keys[RIV_GAMEPAD_UP].down) {
    player_pos.y -= player_speed;
  }
  player_walking = (riv->keys[RIV_GAMEPAD_RIGHT].down || riv->keys[RIV_GAMEPAD_LEFT].down ||
                    riv->keys[RIV_GAMEPAD_DOWN].down || riv->keys[RIV_GAMEPAD_UP].down);
  player_pos.x = clamp(player_pos.x, 0, SCREEN_SIZE-TILE_SIZE);
  player_pos.y = clamp(player_pos.y, TILE_SIZE/2, SCREEN_SIZE-TILE_SIZE-TILE_SIZE/2);

  riv_vec2i player_tilepos = (riv_vec2i){(player_pos.x + TILE_SIZE/2) / TILE_SIZE, (player_pos.y + TILE_SIZE/2) / TILE_SIZE};
  if (map[MAP_LAYER_ITEMS][player_tilepos.y][player_tilepos.x] == SPR_COINS) {
      map[MAP_LAYER_ITEMS][player_tilepos.y][player_tilepos.x] = 0;
      riv_waveform(&(riv_waveform_desc){
        .type = RIV_WAVEFORM_TRIANGLE,
        .attack = 0.025f, .decay = 0.075f, .sustain = 0.075f, .release = 0.075f,
        .start_frequency = 1760.0f,
        .sustain_level = 0.1,
      });
  }
}

void draw() {
  // draw map
  for (int l=0;l<MAP_LAYERS;++l) { // for each layer
    riv->draw.color_key_disabled = l == 0; // disable transparency for ground (optimization)
    for (int y=0;y<MAP_SIZE;++y) { // for each row
      for (int x=0;x<MAP_SIZE;++x) { // for each column
        int spr = map[l][y][x];
        if (spr != 0) { // draw tile sprite
          riv_draw_sprite(spr, SPRITESHEET_GAME, x*TILE_SIZE, y*TILE_SIZE, 1, 1, 1, 1);
        }
      }
    }
  }
  // detect player sprite id
  uint32_t id = (player_walking ? SPR_PLAYER_WALK : SPR_PLAYER_IDLE) + (riv->frame / 4) % NUM_FRAMES_PLAYER;
  // draw player
  riv_draw_sprite(id, SPRITESHEET_GAME, player_pos.x, player_pos.y, 1, 1, player_xflip, 1);
  // draw text
  riv_draw_text("COLLECT THE COINS", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, 128, 24, 1, RIV_COLOR_WHITE);
}

int main() {
  // here we use a slightly modified version of the sprite set
  // "Simple Dungeon Crawler 16x16" by "o_lobster",
  // see https://o-lobster.itch.io/simple-dungeon-crawler-16x16-pixel-pack
  riv_load_palette("simple_dungeon_crawler_16x16.png", 32);
  riv_make_spritesheet(riv_make_image("simple_dungeon_crawler_16x16.png", 0xff), TILE_SIZE, TILE_SIZE);
  do { // main loop
    update();
    draw();
  } while(riv_present());
  return 0;
}
