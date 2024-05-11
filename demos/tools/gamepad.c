//riv-jit-c (DO NOT REMOVE THIS LINE)
#include <riv.h>
#include <math.h>

int main() {
  int img_id = riv_make_image("gamepad.png", RIV_COLOR_BLACK);
  bool redraw = true;
  // configure pal
  riv->draw.pal[RIV_COLOR_LIGHTRED] = RIV_COLOR_LIGHTGREY;
  riv->draw.pal[RIV_COLOR_YELLOW] = RIV_COLOR_WHITE;
  do {
    for (int i=0;i<16;++i) { // detect pressed buttons
      int key = RIV_GAMEPAD_UP + i;
      riv->draw.pal[i+1] = (riv->keys[key].down) ? RIV_COLOR_LIGHTRED : RIV_COLOR_GREY;
      redraw = redraw || riv->keys[key].press || riv->keys[key].release;
      if (riv->keys[key].press) {
        float freq = RIV_NOTE_A4*(1+(i/8.0f));
        riv_waveform(&(riv_waveform_desc){
          .type = RIV_WAVEFORM_SINE,
          .attack = 0.0125f, .decay = 0.025f, .sustain = 0.025f, .release = 0.025f,
          .start_frequency = freq, .end_frequency = freq,
          .sustain_level = 0.2f, .amplitude = 0.25f,
        });
      }
    }
    if (redraw) { // draw
      redraw = false;
      // draw game pad
      riv->draw.pal_enabled = true;
      riv_draw_image_rect(img_id, 0, 0, 256, 256, 0, 0, 1, 1);
      riv->draw.pal_enabled = false;
      // draw left texts
      riv_draw_text("L2/A",     RIV_SPRITESHEET_FONT_5X7, RIV_RIGHT, 53, 88, 1, riv->keys[RIV_GAMEPAD_L2].down ? RIV_COLOR_LIGHTRED : RIV_COLOR_WHITE);
      riv_draw_text("UP",       RIV_SPRITESHEET_FONT_5X7, RIV_RIGHT, 47, 99, 1, riv->keys[RIV_GAMEPAD_UP].down ? RIV_COLOR_LIGHTRED : RIV_COLOR_WHITE);
      riv_draw_text("LEFT",     RIV_SPRITESHEET_FONT_5X7, RIV_RIGHT, 41, 110, 1, riv->keys[RIV_GAMEPAD_LEFT].down ? RIV_COLOR_LIGHTRED : RIV_COLOR_WHITE);
      riv_draw_text("DOWN",     RIV_SPRITESHEET_FONT_5X7, RIV_RIGHT, 47, 121, 1, riv->keys[RIV_GAMEPAD_DOWN].down ? RIV_COLOR_LIGHTRED : RIV_COLOR_WHITE);
      // draw right texts
      riv_draw_text("R2/F",     RIV_SPRITESHEET_FONT_5X7, RIV_LEFT, 202, 88, 1, riv->keys[RIV_GAMEPAD_R2].down ? RIV_COLOR_LIGHTRED : RIV_COLOR_WHITE);
      riv_draw_text("A4/V",     RIV_SPRITESHEET_FONT_5X7, RIV_LEFT, 208, 99, 1, riv->keys[RIV_GAMEPAD_A4].down ? RIV_COLOR_LIGHTRED : RIV_COLOR_WHITE);
      riv_draw_text("A2/X",     RIV_SPRITESHEET_FONT_5X7, RIV_LEFT, 214, 110, 1, riv->keys[RIV_GAMEPAD_A2].down ? RIV_COLOR_LIGHTRED : RIV_COLOR_WHITE);
      riv_draw_text("A1/Z",     RIV_SPRITESHEET_FONT_5X7, RIV_LEFT, 208, 121, 1, riv->keys[RIV_GAMEPAD_A1].down ? RIV_COLOR_LIGHTRED : RIV_COLOR_WHITE);
      // draw top texts
      riv_draw_text("L1/S",     RIV_SPRITESHEET_FONT_5X7, RIV_BOTTOM, 84, 63, 1, riv->keys[RIV_GAMEPAD_L1].down ? RIV_COLOR_LIGHTRED : RIV_COLOR_WHITE);
      riv_draw_text("SELECT/W", RIV_SPRITESHEET_FONT_5X7, RIV_BOTTOM, 108, 48, 1, riv->keys[RIV_GAMEPAD_SELECT].down ? RIV_COLOR_LIGHTRED : RIV_COLOR_WHITE);
      riv_draw_text("START/E",  RIV_SPRITESHEET_FONT_5X7, RIV_BOTTOM, 147, 34, 1, riv->keys[RIV_GAMEPAD_START].down ? RIV_COLOR_LIGHTRED : RIV_COLOR_WHITE);
      riv_draw_text("R1/D",     RIV_SPRITESHEET_FONT_5X7, RIV_BOTTOM, 172, 63, 1, riv->keys[RIV_GAMEPAD_R1].down ? RIV_COLOR_LIGHTRED : RIV_COLOR_WHITE);
      // draw bottom texts
      riv_draw_text("RIGHT",    RIV_SPRITESHEET_FONT_5X7, RIV_TOP, 92, 177, 1, riv->keys[RIV_GAMEPAD_RIGHT].down ? RIV_COLOR_LIGHTRED : RIV_COLOR_WHITE);
      riv_draw_text("L3/Q",     RIV_SPRITESHEET_FONT_5X7, RIV_TOP, 109, 191, 1, riv->keys[RIV_GAMEPAD_L3].down ? RIV_COLOR_LIGHTRED : RIV_COLOR_WHITE);
      riv_draw_text("R3/R",     RIV_SPRITESHEET_FONT_5X7, RIV_TOP, 146, 191, 1, riv->keys[RIV_GAMEPAD_R3].down ? RIV_COLOR_LIGHTRED : RIV_COLOR_WHITE);
      riv_draw_text("A3/C",     RIV_SPRITESHEET_FONT_5X7, RIV_TOP, 162, 177, 1, riv->keys[RIV_GAMEPAD_A3].down ? RIV_COLOR_LIGHTRED : RIV_COLOR_WHITE);
      // draw description
      riv_draw_text("PRESS ANY BUTTON TO TEST IT", RIV_SPRITESHEET_FONT_5X7, RIV_BOTTOM, 128, 256-32, 1, RIV_COLOR_YELLOW);
    }
  } while(riv_present());
  return 0;
}
