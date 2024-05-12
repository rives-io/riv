//riv-jit-c (DO NOT REMOVE THIS LINE)
#include <riv.h>
#include <math.h>
#include <stddef.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define array_len(a) (sizeof(a)/sizeof(a[0]))

enum {
  WAVEFORM_TYPE_COUNT = RIV_WAVEFORM_PULSE,
};

typedef enum panel_type {
  PANEL_PRESET = 0,
  PANEL_TYPE,
  PANEL_ATTACK,
  PANEL_DECAY,
  PANEL_SUSTAIN,
  PANEL_RELEASE,
  PANEL_START_FREQ,
  PANEL_END_FREQ,
  PANEL_AMPLITUDE,
  PANEL_SUSTAIN_LEVEL,
  PANEL_DUTY_CYCLE,
  PANEL_PAN,
  PANEL_COUNT,
} panel_type;

typedef struct note_config {
  const char *name;
  float rel_freq;
  bool sharp;
  uint8_t key;
} note_config;

typedef struct preset_config {
  const char *name;
  riv_waveform_desc waveform;
} preset_config;

// Notes
static note_config notes[] = {
  {"C4", (261.63/261.63), false, RIV_KEYCODE_Q},
  {NULL, (277.18/261.63), true,  RIV_KEYCODE_2},
  {"D4", (293.66/261.63), false, RIV_KEYCODE_W},
  {NULL, (311.13/261.63), true,  RIV_KEYCODE_3},
  {"E4", (329.63/261.63), false, RIV_KEYCODE_E},
  {"F4", (349.23/261.63), false, RIV_KEYCODE_R},
  {NULL, (369.99/261.63), true,  RIV_KEYCODE_5},
  {"G4", (392.00/261.63), false, RIV_KEYCODE_T},
  {NULL, (415.30/261.63), true,  RIV_KEYCODE_6},
  {"A4", (440.00/261.63), false, RIV_KEYCODE_Y},
  {NULL, (466.16/261.63), true,  RIV_KEYCODE_7},
  {"B4", (493.88/261.63), false, RIV_KEYCODE_U},
  {"C5", (523.25/261.63), false, RIV_KEYCODE_I},
  {NULL, (554.37/261.63), true,  RIV_KEYCODE_9},
  {"D5", (587.33/261.63), false, RIV_KEYCODE_O},
  {NULL, (622.25/261.63), true,  RIV_KEYCODE_0},
  {"E5", (659.25/261.63), false, RIV_KEYCODE_P},
  {"F5", (698.46/261.63), false, RIV_KEYCODE_LEFT_BRACKET},
  {NULL, (739.99/261.63), true,  RIV_KEYCODE_EQUAL},
  {"G5", (783.99/261.63), false, RIV_KEYCODE_RIGHT_BRACKET},
  {NULL, (830.61/261.63), true,  RIV_KEYCODE_BACKSPACE},
  {"A5", (880.00/261.63), false, RIV_KEYCODE_BACKSLASH},
  // {NULL, (932.33/261.63), true,  RIV_KEYCODE_},
  // {"B5", (987.77/261.63), false, RIV_KEYCODE_},
};
enum { NUM_NOTES = array_len(notes) };

// Presets
static preset_config presets[] = {
  {"PIANO", {
    .id = 101,
    .type = RIV_WAVEFORM_PULSE,
    .attack = 0.15f,
    .decay = 0.2f,
    .sustain = 0.5f,
    .release = 0.25f,
    .start_frequency = 261.63f,
    .end_frequency = 261.63f,
    .amplitude = 0.25f,
    .sustain_level = 0.3f,
    .duty_cycle = 0.2f,
    .pan = 0.0f,
  }},
  {"SHOOT", {
    .id = 102,
    .type = RIV_WAVEFORM_PULSE,
    .attack = 0.05f,
    .decay = 0.05f,
    .sustain = 0.15f,
    .release = 0.075f,
    .start_frequency = 880.0f,
    .end_frequency = 220.0f,
    .amplitude = 0.15f,
    .sustain_level = 0.25f,
    .duty_cycle = 0.65f,
    .pan = 0.0f,
  }},
  {"JUMP", {
    .id = 103,
    .type = RIV_WAVEFORM_TRIANGLE,
    .attack = 0.025f,
    .decay = 0.1f,
    .sustain = 0.075f,
    .release = 0.025f,
    .start_frequency = 327.0f,
    .end_frequency = 702.0f,
    .amplitude = 0.25f,
    .sustain_level = 0.05f,
    .duty_cycle = 0.5f,
    .pan = 0.0f,
  }},
  {"GAMEOVER", {
    .id = 104,
    .type = RIV_WAVEFORM_PULSE,
    .attack = 0.01f,
    .decay = 0.15f,
    .sustain = 0.1f,
    .release = 0.1f,
    .start_frequency = 110.0f,
    .end_frequency = 22.0f,
    .amplitude = 0.2f,
    .sustain_level = 0.5f,
    .duty_cycle = 0.5f,
    .pan = 0.0f,
  }},
  {"EXPLOSION", {
    .id = 105,
    .type = RIV_WAVEFORM_NOISE,
    .attack = 0.025f,
    .decay = 0.1f,
    .sustain = 0.5f,
    .release = 0.025f,
    .start_frequency = 110.0f,
    .end_frequency = 27.0f,
    .amplitude = 0.25f,
    .sustain_level = 0.5f,
    .duty_cycle = 0.5f,
    .pan = 0.0f,
  }},
  {"HIT", {
    .id = 106,
    .type = RIV_WAVEFORM_NOISE,
    .attack = 0.025f,
    .decay = 0.075f,
    .sustain = 0.075f,
    .release = 0.025f,
    .start_frequency = 250.0f,
    .end_frequency = 100.0f,
    .amplitude = 0.25f,
    .sustain_level = 0.1f,
    .duty_cycle = 0.5f,
    .pan = 0.0f,
  }},
  {"COIN", {
    .id = 107,
    .type = RIV_WAVEFORM_TRIANGLE,
    .attack = 0.025f,
    .decay = 0.075f,
    .sustain = 0.125f,
    .release = 0.120f,
    .start_frequency = 1760.0f,
    .end_frequency = 1760.0f,
    .amplitude = 0.25f,
    .sustain_level = 0.3f,
    .duty_cycle = 0.5f,
    .pan = 0.0f,
  }},
  {"POWERUP", {
    .id = 108,
    .type = RIV_WAVEFORM_PULSE,
    .attack = 0.01f,
    .decay = 0.2f,
    .sustain = 0.3f,
    .release = 0.2f,
    .start_frequency = 55.0f,
    .end_frequency = 1760.0f,
    .amplitude = 0.2f,
    .sustain_level = 0.25f,
    .duty_cycle = 0.125f,
    .pan = 0.0f,
  }},
  {"ERROR", {
    .id = 109,
    .type = RIV_WAVEFORM_SAWTOOTH,
    .attack = 0.075f,
    .decay = 0.15f,
    .sustain = 0.075f,
    .release = 0.225f,
    .start_frequency = 42.0f,
    .end_frequency = 42.0f,
    .amplitude = 0.2f,
    .sustain_level = 0.1f,
    .duty_cycle = 0.5f,
    .pan = 0.0f,
  }},
  {"LASER", {
    .id = 110,
    .type = RIV_WAVEFORM_PULSE,
    .attack = 0.1f,
    .decay = 1.0f,
    .sustain = 0.75f,
    .release = 0.5f,
    .start_frequency = 41.0f,
    .end_frequency = 41.0f,
    .amplitude = 0.15f,
    .sustain_level = 0.5f,
    .duty_cycle = 0.25f,
    .pan = 0.0f,
  }},
};
enum { NUM_PRESETS = array_len(presets) };

static const char *get_waveform_type_name(riv_waveform_type type) {
  switch (type) {
  case RIV_WAVEFORM_SINE: return "SINE";
  case RIV_WAVEFORM_SQUARE: return "SQUARE";
  case RIV_WAVEFORM_TRIANGLE: return "TRIANGLE";
  case RIV_WAVEFORM_SAWTOOTH: return "SAWTOOTH";
  case RIV_WAVEFORM_NOISE: return "NOISE";
  case RIV_WAVEFORM_PULSE: return "PULSE";
  default: return "UNKNOWN";
  }
}

static void draw_panel(const char *title, const char *value, bool focused) {
  riv_draw_text(title, RIV_SPRITESHEET_FONT_5X7, RIV_TOPLEFT, 0, 0, 1, focused ? RIV_COLOR_WHITE : RIV_COLOR_GREY);
  riv_draw_text(value, RIV_SPRITESHEET_FONT_5X7, RIV_TOPLEFT, 96, 0, 1, RIV_COLOR_YELLOW);
  riv->draw.origin.y += 10;
}

static float clamp_quant(float v, float minv, float maxv) {
  if (v <= minv) return minv;
  else if (v >= maxv) return maxv;
  else return (int64_t)(v * 1000.0f + (v < 0 ? -0.5f : 0.5f)) / 1000.0f;
}

static bool is_key_repeat(uint8_t key) {
  if (riv->keys[key].press)
    return true;
  if (riv->keys[key].down && riv->keys[key].down_frame + 20 <= riv->frame && riv->frame % 4 == 0)
    return true;
  return false;
}

static void play_note(int id, float rel_freq, riv_waveform_desc *waveform) {
  riv_waveform_desc note_waveform = *waveform;
  note_waveform.id = id;
  note_waveform.start_frequency = waveform->start_frequency*rel_freq;
  note_waveform.end_frequency = waveform->end_frequency*rel_freq;
  riv_waveform(&note_waveform);
}

int main() {
  bool needs_play = false;
  int focus = 0;
  int preset = 0;
  riv_waveform_desc waveform = presets[preset].waveform;
  for (int i=0;i<NUM_NOTES;++i)
    riv->tracked_keys[notes[i].key] = true;
  do { // main loop
    // handle inputs
    int change = 0;
    if (is_key_repeat(RIV_GAMEPAD_UP)) focus = (focus+PANEL_COUNT-1) % PANEL_COUNT;
    if (is_key_repeat(RIV_GAMEPAD_DOWN)) focus = (focus+PANEL_COUNT+1) % PANEL_COUNT;
    if (is_key_repeat(RIV_GAMEPAD_LEFT)) change--;
    if (is_key_repeat(RIV_GAMEPAD_RIGHT)) change++;
    if (is_key_repeat(RIV_GAMEPAD_A1)) needs_play = true;
    if (is_key_repeat(RIV_GAMEPAD_A2)) {
      waveform.type = riv_rand() % WAVEFORM_TYPE_COUNT + 1;
      float duration = 0.025+riv_rand_float();
      waveform.attack = duration*riv_rand_float();
      waveform.decay = duration*riv_rand_float();
      waveform.sustain = duration*riv_rand_float();
      waveform.release = duration*riv_rand_float();
      waveform.start_frequency = riv_rand_int(RIV_NOTE_C0, RIV_NOTE_A5);
      waveform.end_frequency = riv_rand_int(RIV_NOTE_C0, RIV_NOTE_A5);
      waveform.amplitude = 0.25f;
      waveform.sustain_level = riv_rand_float();
      waveform.duty_cycle = riv_rand_float();
      waveform.pan = 0.0f;
      preset = -1;
      needs_play = true;
    }
    for (int i=0;i<NUM_NOTES;++i) {
      note_config note = notes[i];
      if (riv->keys[note.key].press)
        play_note(1+i, note.rel_freq, &waveform);
      else if (riv->keys[note.key].release)
        play_note(1+i, note.rel_freq, &(riv_waveform_desc){.type = RIV_WAVEFORM_NONE});
    }
    // adjust sound
    if (change != 0) {
      switch (focus) {
      case PANEL_PRESET: preset = (preset + NUM_PRESETS + change) % NUM_PRESETS; waveform = presets[preset].waveform; break;
      case PANEL_TYPE: waveform.type = (((waveform.type-1) + WAVEFORM_TYPE_COUNT + change) % WAVEFORM_TYPE_COUNT) + 1; break;
      case PANEL_ATTACK: waveform.attack = clamp_quant(waveform.attack + change*0.025f, 0, 1); break;
      case PANEL_DECAY: waveform.decay = clamp_quant(waveform.decay + change*0.025f, 0, 1); break;
      case PANEL_SUSTAIN: waveform.sustain = clamp_quant(waveform.sustain + change*0.025f, 0, 1); break;
      case PANEL_RELEASE: waveform.release = clamp_quant(waveform.release + change*0.025f, 0, 1); break;
      case PANEL_START_FREQ: waveform.start_frequency = clamp_quant(waveform.start_frequency*(1.0f + change/25.0f), RIV_NOTE_C0, RIV_NOTE_B8); break;
      case PANEL_END_FREQ: waveform.end_frequency = clamp_quant(waveform.end_frequency*(1.0f + change/25.0f), RIV_NOTE_C0, RIV_NOTE_B8); break;
      case PANEL_AMPLITUDE: waveform.amplitude = clamp_quant(waveform.amplitude + change*0.025f, 0, 1); break;
      case PANEL_SUSTAIN_LEVEL: waveform.sustain_level = clamp_quant(waveform.sustain_level + change*0.025f, 0, 1); break;
      case PANEL_DUTY_CYCLE: waveform.duty_cycle = clamp_quant(waveform.duty_cycle + change*0.025f, 0, 1); break;
      case PANEL_PAN: waveform.pan = clamp_quant(waveform.pan + change*0.025f, -1, 1); break;
      }
      if (focus != PANEL_PRESET) preset = -1;
      needs_play = true;
    }
    if (needs_play && riv->frame % 8 == 0) {
      needs_play = false;
      riv_waveform(&waveform);
    }
    // draw screen
    riv_clear(RIV_COLOR_BLACK); // clear screen
    riv->draw.origin = (riv_vec2i){4, 4};
    riv_draw_text_ex("UP/DOWN    - navigate    Z - play\nLEFT/RIGHT - adjust      X - randomize\nQWERTYUIOP - piano", RIV_SPRITESHEET_FONT_5X7, RIV_BOTTOMLEFT, 0, 256-8, 1, 1, 1, 2, RIV_COLOR_LIGHTGREY);
    draw_panel("PRESET", preset >= 0 ? presets[preset].name : "CUSTOM", focus == PANEL_PRESET);
    draw_panel("WAVEFORM TYPE", get_waveform_type_name(waveform.type), focus == PANEL_TYPE);
    draw_panel("ATTACK", riv_tprintf("%.3f", waveform.attack), focus == PANEL_ATTACK);
    draw_panel("DECAY", riv_tprintf("%.3f", waveform.decay), focus == PANEL_DECAY);
    draw_panel("SUSTAIN", riv_tprintf("%.3f", waveform.sustain), focus == PANEL_SUSTAIN);
    draw_panel("RELEASE", riv_tprintf("%.3f", waveform.release), focus == PANEL_RELEASE);
    draw_panel("START FREQ", riv_tprintf("%.3f", waveform.start_frequency), focus == PANEL_START_FREQ);
    draw_panel("END FREQ", riv_tprintf("%.3f", waveform.end_frequency), focus == PANEL_END_FREQ);
    draw_panel("AMPLITUDE", riv_tprintf("%.3f", waveform.amplitude), focus == PANEL_AMPLITUDE);
    draw_panel("SUSTAIN LEVEL", riv_tprintf("%.3f", waveform.sustain_level), focus == PANEL_SUSTAIN_LEVEL);
    draw_panel("DUTY CYCLE", riv_tprintf("%.3f", waveform.duty_cycle), focus == PANEL_DUTY_CYCLE);
    draw_panel("PAN", riv_tprintf("%.3f", waveform.pan), focus == PANEL_PAN);

    { // ADSR graph
      float x = 0, y = 0;
      float sx = 62, sy = 64;
      float nx, ny;
      riv->draw.origin.y += sy + 2;
      // amplitude
      riv_draw_rect_fill(0, -sy, sx*4, sy, RIV_COLOR_SLATE);
      float l = sy*waveform.amplitude*min((1-waveform.pan), 1);
      float r = sy*waveform.amplitude*min((1+waveform.pan), 1);
      riv_draw_rect_fill(0, -l, sx*2, l, RIV_COLOR_GREY);
      riv_draw_rect_fill(sx*2, -r, sx*2, r, RIV_COLOR_GREY);
      // frequency
      float fs = log2f(waveform.start_frequency) / log2f(RIV_NOTE_B8);
      float fe = log2f(waveform.end_frequency) / log2f(RIV_NOTE_B8);
      riv_draw_line(0, -sy*fs, sx*4-1, -sy*fe, RIV_COLOR_GREEN);
      riv_draw_circle_fill(0, -sy*fs, 3, RIV_COLOR_GREEN);
      riv_draw_circle_fill(sx*4-1, -sy*fe, 3, RIV_COLOR_GREEN);
      // attack
      nx = x + waveform.attack; ny = -1;
      riv_draw_line(x*sx, y*sy, nx*sx, ny*sy, RIV_COLOR_WHITE);
      riv_draw_circle_fill(x*sx, y*sy, 4, RIV_COLOR_RED);
      x = nx; y = ny;
      // decay
      nx = x + waveform.decay; ny = y + (1.0-waveform.sustain_level);
      riv_draw_line(x*sx, y*sy, nx*sx, ny*sy, RIV_COLOR_WHITE);
      riv_draw_circle_fill(x*sx, y*sy, 4, RIV_COLOR_RED);
      x = nx; y = ny;
      // sustain
      nx = x + waveform.sustain;
      riv_draw_line(x*sx, y*sy, nx*sx, ny*sy, RIV_COLOR_WHITE);
      riv_draw_circle_fill(x*sx, y*sy, 4, RIV_COLOR_RED);
      x = nx;
      // release
      nx = x + waveform.release; ny = 0;
      riv_draw_line(x*sx, y*sy, nx*sx, ny*sy, RIV_COLOR_WHITE);
      riv_draw_circle_fill(x*sx, y*sy, 4, RIV_COLOR_RED);
      x = nx; y = ny;
      riv_draw_circle_fill(x*sx, y*sy, 4, RIV_COLOR_RED);
    }

    { // Piano
      riv->draw.origin.y += 4;
      int x = 8;
      for (int i=0;i<NUM_NOTES;++i)  {
        note_config note = notes[i];
        int h = !note.sharp ? 24 : 16;
        int w = !note.sharp ? 12 : 6;
        int col = !note.sharp ? RIV_COLOR_WHITE : RIV_COLOR_GREY;
        if (riv->keys[note.key].down) {
          col = RIV_COLOR_LIGHTRED;
        }
        riv_draw_rect_fill(x, 0, w, h, col);
        if (note.name)
          riv_draw_text(note.name, RIV_SPRITESHEET_FONT_3X5, RIV_LEFT, x+1, h-5, 1, RIV_COLOR_BLACK);
        x += w + 1;
      }
    }
  } while(riv_present()); // refresh screen and wait next frame
  return 0;
}
