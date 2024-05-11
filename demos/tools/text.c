//riv-jit-c (DO NOT REMOVE THIS LINE)
#include <riv.h>
#include <string.h>

// mappings from key to ASCII character
static uint8_t ascii_keymap[RIV_NUM_KEYCODE] = {
  [RIV_KEYCODE_SPACE] = ' ',
  [RIV_KEYCODE_APOSTROPHE] = '\'',  // '
  [RIV_KEYCODE_COMMA] = ',',  // ,
  [RIV_KEYCODE_MINUS] = '-',  // -
  [RIV_KEYCODE_PERIOD] = '.',  // .
  [RIV_KEYCODE_SLASH] = '/',  // /
  [RIV_KEYCODE_0] = '0',
  [RIV_KEYCODE_1] = '1',
  [RIV_KEYCODE_2] = '2',
  [RIV_KEYCODE_3] = '3',
  [RIV_KEYCODE_4] = '4',
  [RIV_KEYCODE_5] = '5',
  [RIV_KEYCODE_6] = '6',
  [RIV_KEYCODE_7] = '7',
  [RIV_KEYCODE_8] = '8',
  [RIV_KEYCODE_9] = '9',
  [RIV_KEYCODE_SEMICOLON] = ';',
  [RIV_KEYCODE_EQUAL] = '=',
  [RIV_KEYCODE_A] = 'a',
  [RIV_KEYCODE_B] = 'b',
  [RIV_KEYCODE_C] = 'c',
  [RIV_KEYCODE_D] = 'd',
  [RIV_KEYCODE_E] = 'e',
  [RIV_KEYCODE_F] = 'f',
  [RIV_KEYCODE_G] = 'g',
  [RIV_KEYCODE_H] = 'h',
  [RIV_KEYCODE_I] = 'i',
  [RIV_KEYCODE_J] = 'j',
  [RIV_KEYCODE_K] = 'k',
  [RIV_KEYCODE_L] = 'l',
  [RIV_KEYCODE_M] = 'm',
  [RIV_KEYCODE_N] = 'n',
  [RIV_KEYCODE_O] = 'o',
  [RIV_KEYCODE_P] = 'p',
  [RIV_KEYCODE_Q] = 'q',
  [RIV_KEYCODE_R] = 'r',
  [RIV_KEYCODE_S] = 's',
  [RIV_KEYCODE_T] = 't',
  [RIV_KEYCODE_U] = 'u',
  [RIV_KEYCODE_V] = 'v',
  [RIV_KEYCODE_W] = 'w',
  [RIV_KEYCODE_X] = 'x',
  [RIV_KEYCODE_Y] = 'y',
  [RIV_KEYCODE_Z] = 'z',
  [RIV_KEYCODE_LEFT_BRACKET] = '[',
  [RIV_KEYCODE_BACKSLASH] = '\\',
  [RIV_KEYCODE_RIGHT_BRACKET] = ']',
  [RIV_KEYCODE_GRAVE_ACCENT] = '`',
  [RIV_KEYCODE_TAB] = '\t',
  [RIV_KEYCODE_ENTER] = '\n',
};

// mappings from SHIFT + key to ASCII character
static uint8_t ascii_shift_keymap[RIV_NUM_KEYCODE] = {
  [RIV_KEYCODE_SPACE] = ' ',
  [RIV_KEYCODE_APOSTROPHE] = '"',  // '
  [RIV_KEYCODE_COMMA] = '<',  // ,
  [RIV_KEYCODE_MINUS] = '_',  // -
  [RIV_KEYCODE_PERIOD] = '>',  // .
  [RIV_KEYCODE_SLASH] = '?',  // /
  [RIV_KEYCODE_0] = ')',
  [RIV_KEYCODE_1] = '!',
  [RIV_KEYCODE_2] = '@',
  [RIV_KEYCODE_3] = '#',
  [RIV_KEYCODE_4] = '$',
  [RIV_KEYCODE_5] = '%',
  [RIV_KEYCODE_6] = '^',
  [RIV_KEYCODE_7] = '&',
  [RIV_KEYCODE_8] = '*',
  [RIV_KEYCODE_9] = '(',
  [RIV_KEYCODE_SEMICOLON] = ':',
  [RIV_KEYCODE_EQUAL] = '+',
  [RIV_KEYCODE_A] = 'A',
  [RIV_KEYCODE_B] = 'B',
  [RIV_KEYCODE_C] = 'C',
  [RIV_KEYCODE_D] = 'D',
  [RIV_KEYCODE_E] = 'E',
  [RIV_KEYCODE_F] = 'F',
  [RIV_KEYCODE_G] = 'G',
  [RIV_KEYCODE_H] = 'H',
  [RIV_KEYCODE_I] = 'I',
  [RIV_KEYCODE_J] = 'J',
  [RIV_KEYCODE_K] = 'K',
  [RIV_KEYCODE_L] = 'L',
  [RIV_KEYCODE_M] = 'M',
  [RIV_KEYCODE_N] = 'N',
  [RIV_KEYCODE_O] = 'O',
  [RIV_KEYCODE_P] = 'P',
  [RIV_KEYCODE_Q] = 'Q',
  [RIV_KEYCODE_R] = 'R',
  [RIV_KEYCODE_S] = 'S',
  [RIV_KEYCODE_T] = 'T',
  [RIV_KEYCODE_U] = 'U',
  [RIV_KEYCODE_V] = 'V',
  [RIV_KEYCODE_W] = 'W',
  [RIV_KEYCODE_X] = 'X',
  [RIV_KEYCODE_Y] = 'Y',
  [RIV_KEYCODE_Z] = 'Z',
  [RIV_KEYCODE_LEFT_BRACKET] = '{',
  [RIV_KEYCODE_BACKSLASH] = '|',
  [RIV_KEYCODE_RIGHT_BRACKET] = '}',
  [RIV_KEYCODE_GRAVE_ACCENT] = '~',
  [RIV_KEYCODE_TAB] = '\t',
  [RIV_KEYCODE_ENTER] = '\n',
};

static void glyphs_draw_glyphs(int x0, int y0, uint64_t sps) {
  int x = x0;
  int y = y0;
  for(int i = 0; i < 256; i += 1) {
    riv_draw_sprite(i, sps, x, y, 1, 1, 1, 1);
    x = (x + 9);
    if(i % 16 == 15) {
      x = x0;
      y = (y + 8);
    }
  }
}

static bool is_key_repeat(uint8_t key) {
  return (riv->keys[key].press) ||
         (riv->keys[key].down && riv->keys[key].down_frame + 20 <= riv->frame && riv->frame % 4 == 0);
}

int main() {
  char text[1024] = "TYPE WITH THE KEYBOARD TO EDIT THIS TEXT\n\
HELLO WORLD!\n\
hello world!\n\
\n\
abcdefghijklmnopqrstuvwxyz\n\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\n\
0123456789\n";
  int cursor = strlen(text);
  // track all keys
  for (int i=0;i<RIV_NUM_KEYCODE;++i) {
    riv->tracked_keys[i] = true;
  }
  // main loop
  do {
    riv_clear(RIV_COLOR_BLACK);
    if (is_key_repeat(RIV_KEYCODE_BACKSPACE)) {
      if (cursor > 0) {
        text[--cursor] = 0;
      }
    } else {
      for (int key=0;key<RIV_NUM_KEYCODE;++key) {
        if (is_key_repeat(key)) {
          bool shift = riv->keys[RIV_KEYCODE_LEFT_SHIFT].down || riv->keys[RIV_KEYCODE_RIGHT_SHIFT].down;
          int ch = shift ? ascii_shift_keymap[key] : ascii_keymap[key];
          if (ch > 0 && cursor < (int)sizeof(text) - 4) {
            text[cursor] = ch;
            text[++cursor] = 0;
          }
        }
      }
    }
    if ((riv->frame / 24) % 2 == 0) {
      text[cursor] = '\x1f';
      text[cursor+1] = 0;
    } else {
      text[cursor] = ' ';
      text[cursor+1] = 0;
    }
    riv_draw_text("BIG FONT - 5x7 pixels", RIV_SPRITESHEET_FONT_5X7, RIV_TOP, 128, 4, 1, RIV_COLOR_YELLOW);
    glyphs_draw_glyphs(56, 16, RIV_SPRITESHEET_FONT_5X7);
    riv_draw_text("SMALL FONT - 3x5 pixels", RIV_SPRITESHEET_FONT_3X5, RIV_TOP, 128, 88, 1, RIV_COLOR_YELLOW);
    glyphs_draw_glyphs(56, 98, RIV_SPRITESHEET_FONT_3X5);
    riv_vec2i text_size = riv_draw_text(text, RIV_SPRITESHEET_FONT_5X7, RIV_TOPLEFT, 8, 168, 1, 1);
    riv_draw_rect_line(6, 166, (text_size.x + 4), (text_size.y + 4), RIV_COLOR_RED);
  } while(riv_present());
  return 0;
}
