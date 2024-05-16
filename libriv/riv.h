#ifndef RIV_H
#define RIV_H

#ifndef RIV_NO_INCLUDES
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#endif

#define RIV_VERSION_MAJOR 0
#define RIV_VERSION_MINOR 3
#define RIV_VERSION_PATCH 0
#define RIV_VERSION (RIV_VERSION_MAJOR*1000000 + RIV_VERSION_MINOR*1000 + RIV_VERSION_PATCH)

#ifndef RIV_API
#define RIV_API
#endif

////////////////////////////////////////////////////////////////////////////////
// Constants

// All supported RIV key codes
typedef enum riv_key_code {
  RIV_KEYCODE_INVALID             = 0,
  // F keys
  RIV_KEYCODE_F1                  = 1,
  RIV_KEYCODE_F2                  = 2,
  RIV_KEYCODE_F3                  = 3,
  RIV_KEYCODE_F4                  = 4,
  RIV_KEYCODE_F5                  = 5,
  RIV_KEYCODE_F6                  = 6,
  RIV_KEYCODE_F7                  = 7,
  RIV_KEYCODE_F8                  = 8,
  RIV_KEYCODE_F9                  = 9,
  RIV_KEYCODE_F10                 = 10,
  RIV_KEYCODE_F11                 = 11,
  RIV_KEYCODE_F12                 = 12,
  // numeric keys
  RIV_KEYCODE_BACKSPACE           = 13,
  RIV_KEYCODE_NUM_LOCK            = 14,
  RIV_KEYCODE_KP_ENTER            = 15,
  RIV_KEYCODE_KP_0                = 16,
  RIV_KEYCODE_KP_1                = 17,
  RIV_KEYCODE_KP_2                = 18,
  RIV_KEYCODE_KP_3                = 19,
  RIV_KEYCODE_KP_4                = 20,
  RIV_KEYCODE_KP_5                = 21,
  RIV_KEYCODE_KP_6                = 22,
  RIV_KEYCODE_KP_7                = 23,
  RIV_KEYCODE_KP_8                = 24,
  RIV_KEYCODE_KP_9                = 25,
  RIV_KEYCODE_KP_DECIMAL          = 26,
  RIV_KEYCODE_KP_DIVIDE           = 27,
  RIV_KEYCODE_KP_MULTIPLY         = 28,
  RIV_KEYCODE_KP_SUBTRACT         = 29,
  RIV_KEYCODE_KP_ADD              = 30,
  RIV_KEYCODE_KP_EQUAL            = 31,
  // printable keys
  RIV_KEYCODE_SPACE               = 32,
  // ASCII 33 !
  // ASCII 34 "
  // ASCII 35 #
  // ASCII 36 $
  // ASCII 37 %
  // ASCII 38 &
  RIV_KEYCODE_APOSTROPHE          = 39,  // '
  // ASCII 40 (
  // ASCII 41 )
  // ASCII 42 *
  // ASCII 43 +
  RIV_KEYCODE_COMMA               = 44,  // ,
  RIV_KEYCODE_MINUS               = 45,  // -
  RIV_KEYCODE_PERIOD              = 46,  // .
  RIV_KEYCODE_SLASH               = 47,  // /
  RIV_KEYCODE_0                   = 48,
  RIV_KEYCODE_1                   = 49,
  RIV_KEYCODE_2                   = 50,
  RIV_KEYCODE_3                   = 51,
  RIV_KEYCODE_4                   = 52,
  RIV_KEYCODE_5                   = 53,
  RIV_KEYCODE_6                   = 54,
  RIV_KEYCODE_7                   = 55,
  RIV_KEYCODE_8                   = 56,
  RIV_KEYCODE_9                   = 57,
  // ASCII 58 :
  RIV_KEYCODE_SEMICOLON           = 59,  // ;
  // ASCII 60 <
  RIV_KEYCODE_EQUAL               = 61,  // =
  // ASCII 62 >
  // ASCII 63 ?
  // ASCII 64 @
  RIV_KEYCODE_A                   = 65,
  RIV_KEYCODE_B                   = 66,
  RIV_KEYCODE_C                   = 67,
  RIV_KEYCODE_D                   = 68,
  RIV_KEYCODE_E                   = 69,
  RIV_KEYCODE_F                   = 70,
  RIV_KEYCODE_G                   = 71,
  RIV_KEYCODE_H                   = 72,
  RIV_KEYCODE_I                   = 73,
  RIV_KEYCODE_J                   = 74,
  RIV_KEYCODE_K                   = 75,
  RIV_KEYCODE_L                   = 76,
  RIV_KEYCODE_M                   = 77,
  RIV_KEYCODE_N                   = 78,
  RIV_KEYCODE_O                   = 79,
  RIV_KEYCODE_P                   = 80,
  RIV_KEYCODE_Q                   = 81,
  RIV_KEYCODE_R                   = 82,
  RIV_KEYCODE_S                   = 83,
  RIV_KEYCODE_T                   = 84,
  RIV_KEYCODE_U                   = 85,
  RIV_KEYCODE_V                   = 86,
  RIV_KEYCODE_W                   = 87,
  RIV_KEYCODE_X                   = 88,
  RIV_KEYCODE_Y                   = 89,
  RIV_KEYCODE_Z                   = 90,
  RIV_KEYCODE_LEFT_BRACKET        = 91,  // [
  RIV_KEYCODE_BACKSLASH           = 92,  // '\'
  RIV_KEYCODE_RIGHT_BRACKET       = 93,  // ]
  // ASCII 94 ^
  // ASCII 95 _
  RIV_KEYCODE_GRAVE_ACCENT        = 96,  // `
  // other keys
  RIV_KEYCODE_ESCAPE              = 97,
  RIV_KEYCODE_ENTER               = 98,
  RIV_KEYCODE_TAB                 = 99,
  RIV_KEYCODE_INSERT              = 100,
  RIV_KEYCODE_DELETE              = 101,
  RIV_KEYCODE_UP                  = 102,
  RIV_KEYCODE_DOWN                = 103,
  RIV_KEYCODE_LEFT                = 104,
  RIV_KEYCODE_RIGHT               = 105,
  RIV_KEYCODE_PAGE_UP             = 106,
  RIV_KEYCODE_PAGE_DOWN           = 107,
  RIV_KEYCODE_HOME                = 108,
  RIV_KEYCODE_END                 = 109,
  RIV_KEYCODE_CAPS_LOCK           = 110,
  RIV_KEYCODE_SCROLL_LOCK         = 111,
  RIV_KEYCODE_PRINT_SCREEN        = 112,
  RIV_KEYCODE_PAUSE               = 113,
  RIV_KEYCODE_LEFT_SHIFT          = 114,
  RIV_KEYCODE_LEFT_CTRL           = 115,
  RIV_KEYCODE_LEFT_ALT            = 116,
  RIV_KEYCODE_LEFT_SUPER          = 117,
  RIV_KEYCODE_RIGHT_SHIFT         = 118,
  RIV_KEYCODE_RIGHT_CTRL          = 119,
  RIV_KEYCODE_RIGHT_ALT           = 120,
  RIV_KEYCODE_RIGHT_SUPER         = 121,
  RIV_KEYCODE_MENU                = 122,
  // ASCII 123 {
  // ASCII 124 |
  // ASCII 125 }
  // ASCII 126 ~
  // 127 is reserved
  // Game pad 1
  RIV_GAMEPAD1_UP                  = 128, // Alias for Keyboard Up
  RIV_GAMEPAD1_DOWN                = 129, // Alias for Keyboard Down
  RIV_GAMEPAD1_LEFT                = 130, // Alias for Keyboard Left
  RIV_GAMEPAD1_RIGHT               = 131, // Alias for Keyboard Right
  RIV_GAMEPAD1_A1                  = 132, // Alias for Keyboard Z
  RIV_GAMEPAD1_A2                  = 133, // Alias for Keyboard X
  RIV_GAMEPAD1_A3                  = 134, // Alias for Keyboard C
  RIV_GAMEPAD1_A4                  = 135, // Alias for Keyboard V
  RIV_GAMEPAD1_L1                  = 136, // Alias for Keyboard S
  RIV_GAMEPAD1_R1                  = 137, // Alias for Keyboard D
  RIV_GAMEPAD1_L2                  = 138, // Alias for Keyboard A
  RIV_GAMEPAD1_R2                  = 139, // Alias for Keyboard F
  RIV_GAMEPAD1_SELECT              = 140, // Alias for Keyboard W
  RIV_GAMEPAD1_START               = 141, // Alias for Keyboard E
  RIV_GAMEPAD1_L3                  = 142, // Alias for Keyboard Q
  RIV_GAMEPAD1_R3                  = 143, // Alias for Keyboard R
  // Game pad 2
  RIV_GAMEPAD2_UP                  = 144,
  RIV_GAMEPAD2_DOWN                = 145,
  RIV_GAMEPAD2_LEFT                = 146,
  RIV_GAMEPAD2_RIGHT               = 147,
  RIV_GAMEPAD2_A1                  = 148,
  RIV_GAMEPAD2_A2                  = 149,
  RIV_GAMEPAD2_A3                  = 150,
  RIV_GAMEPAD2_A4                  = 151,
  RIV_GAMEPAD2_L1                  = 152,
  RIV_GAMEPAD2_R1                  = 153,
  RIV_GAMEPAD2_L2                  = 154,
  RIV_GAMEPAD2_R2                  = 155,
  RIV_GAMEPAD2_SELECT              = 156,
  RIV_GAMEPAD2_START               = 157,
  RIV_GAMEPAD2_L3                  = 158,
  RIV_GAMEPAD2_R3                  = 159,
  // Game pad 3
  RIV_GAMEPAD3_UP                  = 160,
  RIV_GAMEPAD3_DOWN                = 161,
  RIV_GAMEPAD3_LEFT                = 162,
  RIV_GAMEPAD3_RIGHT               = 163,
  RIV_GAMEPAD3_A1                  = 164,
  RIV_GAMEPAD3_A2                  = 165,
  RIV_GAMEPAD3_A3                  = 166,
  RIV_GAMEPAD3_A4                  = 167,
  RIV_GAMEPAD3_L1                  = 168,
  RIV_GAMEPAD3_R1                  = 169,
  RIV_GAMEPAD3_L2                  = 170,
  RIV_GAMEPAD3_R2                  = 171,
  RIV_GAMEPAD3_SELECT              = 172,
  RIV_GAMEPAD3_START               = 173,
  RIV_GAMEPAD3_L3                  = 174,
  RIV_GAMEPAD3_R3                  = 175,
  // Game pad 4
  RIV_GAMEPAD4_UP                  = 176,
  RIV_GAMEPAD4_DOWN                = 177,
  RIV_GAMEPAD4_LEFT                = 178,
  RIV_GAMEPAD4_RIGHT               = 179,
  RIV_GAMEPAD4_A1                  = 180,
  RIV_GAMEPAD4_A2                  = 181,
  RIV_GAMEPAD4_A3                  = 182,
  RIV_GAMEPAD4_A4                  = 183,
  RIV_GAMEPAD4_L1                  = 184,
  RIV_GAMEPAD4_R1                  = 185,
  RIV_GAMEPAD4_L2                  = 186,
  RIV_GAMEPAD4_R2                  = 187,
  RIV_GAMEPAD4_SELECT              = 188,
  RIV_GAMEPAD4_START               = 189,
  RIV_GAMEPAD4_L3                  = 190,
  RIV_GAMEPAD4_R3                  = 191,
  // Number of keys
  RIV_NUM_KEYCODE                  = 192,
  // Aliases
  RIV_GAMEPAD_UP                   = RIV_GAMEPAD1_UP,
  RIV_GAMEPAD_DOWN                 = RIV_GAMEPAD1_DOWN,
  RIV_GAMEPAD_LEFT                 = RIV_GAMEPAD1_LEFT,
  RIV_GAMEPAD_RIGHT                = RIV_GAMEPAD1_RIGHT,
  RIV_GAMEPAD_A1                   = RIV_GAMEPAD1_A1,
  RIV_GAMEPAD_A2                   = RIV_GAMEPAD1_A2,
  RIV_GAMEPAD_A3                   = RIV_GAMEPAD1_A3,
  RIV_GAMEPAD_A4                   = RIV_GAMEPAD1_A4,
  RIV_GAMEPAD_L1                   = RIV_GAMEPAD1_L1,
  RIV_GAMEPAD_R1                   = RIV_GAMEPAD1_R1,
  RIV_GAMEPAD_L2                   = RIV_GAMEPAD1_L2,
  RIV_GAMEPAD_R2                   = RIV_GAMEPAD1_R2,
  RIV_GAMEPAD_SELECT               = RIV_GAMEPAD1_SELECT,
  RIV_GAMEPAD_START                = RIV_GAMEPAD1_START,
  RIV_GAMEPAD_L3                   = RIV_GAMEPAD1_L3,
  RIV_GAMEPAD_R3                   = RIV_GAMEPAD1_R3,
} riv_key_code;

// Note frequencies
typedef enum riv_note_freq {
  RIV_NOTE_C0  = 16,
  RIV_NOTE_Db0 = 17,
  RIV_NOTE_D0  = 18,
  RIV_NOTE_Eb0 = 19,
  RIV_NOTE_E0  = 20,
  RIV_NOTE_F0  = 21,
  RIV_NOTE_Gb0 = 23,
  RIV_NOTE_G0  = 24,
  RIV_NOTE_Ab0 = 25,
  RIV_NOTE_A0  = 27,
  RIV_NOTE_Bb0 = 29,
  RIV_NOTE_B0  = 30,
  RIV_NOTE_C1  = 32,
  RIV_NOTE_Db1 = 34,
  RIV_NOTE_D1  = 36,
  RIV_NOTE_Eb1 = 38,
  RIV_NOTE_E1  = 41,
  RIV_NOTE_F1  = 43,
  RIV_NOTE_Gb1 = 46,
  RIV_NOTE_G1  = 49,
  RIV_NOTE_Ab1 = 51,
  RIV_NOTE_A1  = 55,
  RIV_NOTE_Bb1 = 58,
  RIV_NOTE_B1  = 61,
  RIV_NOTE_C2  = 65,
  RIV_NOTE_Db2 = 69,
  RIV_NOTE_D2  = 73,
  RIV_NOTE_Eb2 = 77,
  RIV_NOTE_E2  = 82,
  RIV_NOTE_F2  = 87,
  RIV_NOTE_Gb2 = 92,
  RIV_NOTE_G2  = 98,
  RIV_NOTE_Ab2 = 103,
  RIV_NOTE_A2  = 110,
  RIV_NOTE_Bb2 = 116,
  RIV_NOTE_B2  = 123,
  RIV_NOTE_C3  = 130,
  RIV_NOTE_Db3 = 138,
  RIV_NOTE_D3  = 146,
  RIV_NOTE_Eb3 = 155,
  RIV_NOTE_E3  = 164,
  RIV_NOTE_F3  = 174,
  RIV_NOTE_Gb3 = 185,
  RIV_NOTE_G3  = 196,
  RIV_NOTE_Ab3 = 207,
  RIV_NOTE_A3  = 220,
  RIV_NOTE_Bb3 = 233,
  RIV_NOTE_B3  = 246,
  RIV_NOTE_C4  = 261,
  RIV_NOTE_Db4 = 277,
  RIV_NOTE_D4  = 293,
  RIV_NOTE_Eb4 = 311,
  RIV_NOTE_E4  = 329,
  RIV_NOTE_F4  = 349,
  RIV_NOTE_Gb4 = 369,
  RIV_NOTE_G4  = 392,
  RIV_NOTE_Ab4 = 415,
  RIV_NOTE_A4  = 440,
  RIV_NOTE_Bb4 = 466,
  RIV_NOTE_B4  = 493,
  RIV_NOTE_C5  = 523,
  RIV_NOTE_Db5 = 554,
  RIV_NOTE_D5  = 587,
  RIV_NOTE_Eb5 = 622,
  RIV_NOTE_E5  = 659,
  RIV_NOTE_F5  = 698,
  RIV_NOTE_Gb5 = 739,
  RIV_NOTE_G5  = 783,
  RIV_NOTE_Ab5 = 830,
  RIV_NOTE_A5  = 880,
  RIV_NOTE_Bb5 = 932,
  RIV_NOTE_B5  = 987,
  RIV_NOTE_C6  = 1046,
  RIV_NOTE_Db6 = 1108,
  RIV_NOTE_D6  = 1174,
  RIV_NOTE_Eb6 = 1244,
  RIV_NOTE_E6  = 1318,
  RIV_NOTE_F6  = 1396,
  RIV_NOTE_Gb6 = 1479,
  RIV_NOTE_G6  = 1567,
  RIV_NOTE_Ab6 = 1661,
  RIV_NOTE_A6  = 1760,
  RIV_NOTE_Bb6 = 1864,
  RIV_NOTE_B6  = 1975,
  RIV_NOTE_C7  = 2093,
  RIV_NOTE_Db7 = 2217,
  RIV_NOTE_D7  = 2349,
  RIV_NOTE_Eb7 = 2489,
  RIV_NOTE_E7  = 2637,
  RIV_NOTE_F7  = 2793,
  RIV_NOTE_Gb7 = 2959,
  RIV_NOTE_G7  = 3135,
  RIV_NOTE_Ab7 = 3322,
  RIV_NOTE_A7  = 3520,
  RIV_NOTE_Bb7 = 3729,
  RIV_NOTE_B7  = 3951,
  RIV_NOTE_C8  = 4186,
  RIV_NOTE_Db8 = 4434,
  RIV_NOTE_D8  = 4698,
  RIV_NOTE_Eb8 = 4978,
  RIV_NOTE_E8  = 5274,
  RIV_NOTE_F8  = 5587,
  RIV_NOTE_Gb8 = 5919,
  RIV_NOTE_G8  = 6271,
  RIV_NOTE_Ab8 = 6644,
  RIV_NOTE_A8  = 7040,
  RIV_NOTE_Bb8 = 7458,
  RIV_NOTE_B8  = 7902,
} riv_note_freq;

// Default palette color indexes
typedef enum riv_color_id {
  RIV_COLOR_BLACK       = 0,
  RIV_COLOR_WHITE       = 1,
  RIV_COLOR_LIGHTGREY   = 2,
  RIV_COLOR_GREY        = 3,
  RIV_COLOR_DARKSLATE   = 4,
  RIV_COLOR_SLATE       = 5,
  RIV_COLOR_LIGHTSLATE  = 6,
  RIV_COLOR_LIGHTTEAL   = 7,
  RIV_COLOR_LIGHTBLUE   = 8,
  RIV_COLOR_BLUE        = 9,
  RIV_COLOR_DARKERGREEN = 10,
  RIV_COLOR_DARKGREEN   = 11,
  RIV_COLOR_GREEN       = 12,
  RIV_COLOR_LIGHTGREEN  = 13,
  RIV_COLOR_LIGHTYELLOW = 14,
  RIV_COLOR_LIGHTPEACH  = 15,
  RIV_COLOR_PEACH       = 16,
  RIV_COLOR_LIGHTBROWN  = 17,
  RIV_COLOR_BROWN       = 18,
  RIV_COLOR_DARKBROWN   = 19,
  RIV_COLOR_DARKERBROWN = 20,
  RIV_COLOR_DARKRED     = 21,
  RIV_COLOR_RED         = 22,
  RIV_COLOR_LIGHTRED    = 23,
  RIV_COLOR_ORANGE      = 24,
  RIV_COLOR_GOLD        = 25,
  RIV_COLOR_YELLOW      = 26,
  RIV_COLOR_LIGHTPINK   = 27,
  RIV_COLOR_PINK        = 28,
  RIV_COLOR_DARKPINK    = 29,
  RIV_COLOR_PURPLE      = 30,
  RIV_COLOR_DARKPURPLE  = 31,
  RIV_NUM_COLORS        = 32,
} riv_color_id;

// Default palette colors
typedef enum riv_color_rgb_code {
  RIV_RGB_BLACK       = 0x000000,
  RIV_RGB_WHITE       = 0xFFFFFF,
  RIV_RGB_LIGHTGREY   = 0xDBCDC5,
  RIV_RGB_GREY        = 0x726B68,
  RIV_RGB_DARKSLATE   = 0x251418,
  RIV_RGB_SLATE       = 0x583635,
  RIV_RGB_LIGHTSLATE  = 0xB6978B,
  RIV_RGB_LIGHTTEAL   = 0xE9E95E,
  RIV_RGB_LIGHTBLUE   = 0xDC9028,
  RIV_RGB_BLUE        = 0xA73118,
  RIV_RGB_DARKERGREEN = 0x393205,
  RIV_RGB_DARKGREEN   = 0x415F00,
  RIV_RGB_GREEN       = 0x3BB208,
  RIV_RGB_LIGHTGREEN  = 0x41F647,
  RIV_RGB_LIGHTYELLOW = 0x75FFE8,
  RIV_RGB_LIGHTPEACH  = 0x82BEFB,
  RIV_RGB_PEACH       = 0x5197DE,
  RIV_RGB_LIGHTBROWN  = 0x3168B6,
  RIV_RGB_BROWN       = 0x26498A,
  RIV_RGB_DARKBROWN   = 0x141C46,
  RIV_RGB_DARKERBROWN = 0x0D091E,
  RIV_RGB_DARKRED     = 0x0D0D72,
  RIV_RGB_RED         = 0x2424DA,
  RIV_RGB_LIGHTRED    = 0x4400FF,
  RIV_RGB_ORANGE      = 0x106EEF,
  RIV_RGB_GOLD        = 0x11ABEC,
  RIV_RGB_YELLOW      = 0x10E9EC,
  RIV_RGB_LIGHTPINK   = 0x8D8DF7,
  RIV_RGB_PINK        = 0x6D4EF9,
  RIV_RGB_DARKPINK    = 0x5824C1,
  RIV_RGB_PURPLE      = 0x521284,
  RIV_RGB_DARKPURPLE  = 0x3B083D,
} riv_rgb_code;

// Pixel format for frame buffers
typedef enum riv_pixel_format {
  RIV_PIXELFORMAT_PLT256,
  RIV_NUM_PIXELFORMAT,
} riv_pixel_format;

// Waveform types
typedef enum riv_waveform_type {
  RIV_WAVEFORM_NONE = 0,
  RIV_WAVEFORM_SINE,
  RIV_WAVEFORM_SQUARE,
  RIV_WAVEFORM_TRIANGLE,
  RIV_WAVEFORM_SAWTOOTH,
  RIV_WAVEFORM_NOISE,
  RIV_WAVEFORM_PULSE,
} riv_waveform_type;

// Sound format for sound buffers
typedef enum riv_sound_format {
  RIV_SOUNDFORMAT_NONE = 0,
  RIV_SOUNDFORMAT_U8,       // 8-bit unsigned integer                 [0, 255]
  RIV_SOUNDFORMAT_S16,      // 16-bit signed integer                  [-32768, 32767]
  RIV_SOUNDFORMAT_S24,      // 24-bit signed integer (tightly packed) [-8388608, 8388607]
  RIV_SOUNDFORMAT_S32,      // 32-bit signed integer                  [-2147483648, 2147483647]
  RIV_SOUNDFORMAT_F32,      // 32-bit floating point                  [-1, 1]
  RIV_SOUNDFORMAT_WAV,      // WAV file
  RIV_SOUNDFORMAT_FLAC,     // FLAC file
  RIV_SOUNDFORMAT_MP3,      // MP3 file
  RIV_SOUNDFORMAT_OGG,      // OGG Vorbis file
} riv_sound_format;

// Default constants
typedef enum riv_constants {
  RIV_DEFAULT_WIDTH = 256,
  RIV_DEFAULT_HEIGHT = 256,
  RIV_DEFAULT_TARGET_FPS = 60,
  RIV_DEFAULT_PIXELFORMAT = RIV_PIXELFORMAT_PLT256,
  RIV_MAX_COLORS = 256,
  RIV_MAX_IMAGES = 4096,
  RIV_MAX_SPRITESHEETS = 4096,
  RIV_MAX_KEY_TOGGLES = 64,
  RIV_MAX_COMMANDS = 32,
  RIV_NUM_GAMEPADS = 4,
  RIV_NUM_GAMEPAD_BUTTONS = 16,
  RIV_INVALID_ID = 0,
} riv_constants;

// Predefined image ids
typedef enum riv_image_id {
  RIV_IMAGE_NONE = 0,
  RIV_IMAGE_FONT_5X7 = RIV_MAX_IMAGES-1,
  RIV_IMAGE_FONT_3X5 = RIV_MAX_IMAGES-2,
} riv_image_id;

// Predefined sprite ids
typedef enum riv_spritesheet_id {
  RIV_SPRITESHEET_NONE = 0,
  RIV_SPRITESHEET_FONT_5X7 = RIV_MAX_SPRITESHEETS-1,
  RIV_SPRITESHEET_FONT_3X5 = RIV_MAX_SPRITESHEETS-2,
} riv_spritesheet_id;

// Alignment
typedef enum riv_align {
  RIV_TOPLEFT = 0,
  RIV_TOP,
  RIV_TOPRIGHT,
  RIV_LEFT,
  RIV_CENTER,
  RIV_RIGHT,
  RIV_BOTTOMLEFT,
  RIV_BOTTOM,
  RIV_BOTTOMRIGHT,
} riv_align;

////////////////////////////////////////////////////////////////////////////////
// Constants used internally for driver and device communication

// Device control commands
typedef enum riv_control_command {
  RIV_CONTROL_NONE = 0,
  RIV_CONTROL_OPEN,
  RIV_CONTROL_CLOSE,
  RIV_CONTROL_PRESENT,
  RIV_CONTROL_FLUSH_COMMAND,
} riv_control_command;

// Device audio commands
typedef enum riv_command_type {
  RIV_COMMAND_NONE = 0,
  RIV_COMMAND_MAKE_SOUNDBUFFER,
  RIV_COMMAND_DESTROY_SOUNDBUFFER,
  RIV_COMMAND_SOUND,
  RIV_COMMAND_WAVEFORM,
} riv_command_type;

// Memory mapped sizes
typedef enum riv_mem_size {
  RIV_SIZE_CONTEXT      = 12*1024*1024, // 12 MB
  RIV_SIZE_MMIO_DRIVER  =       4*1024, // 4 KB
  RIV_SIZE_MMIO_DEVICE  =       4*1024, // 4 KB
  RIV_SIZE_PUB_DRIVER   =    1016*1024, // 1016 KB
  RIV_SIZE_PRV_DRIVER   =    1024*1024, // 1 MB
  RIV_SIZE_INCARD       =  2*1024*1024, // 2 MB
  RIV_SIZE_OUTCARD      =  2*1024*1024, // 2 MB
  RIV_SIZE_STATECARD    =  2*1024*1024, // 2 MB
  RIV_SIZE_FRAMEBUFFER  =  2*1024*1024, // 2 MB
  RIV_SIZE_TXBUFFER     =  2*1024*1024, // 2 MB
  RIV_SIZE_TEMP_STRBUF  =  64*1024,     // 64KB
} riv_mem_size;

// Memory mapped virtual address
typedef enum riv_mem_addr {
  RIV_VADDR_CONTEXT      = 0x10000000, // Context fixed address
  RIV_VADDR_MMIO_DRIVER  = 0x10000000, // Driver memory mapped IO fixed address
  RIV_VADDR_MMIO_DEVICE  = 0x10001000, // Device memory mapped IO fixed address
  RIV_VADDR_PUB_DRIVER   = 0x10002000, // Public driver region fixed address
  RIV_VADDR_PRV_DRIVER   = 0x10100000, // Private driver region fixed address
  RIV_VADDR_INCARD       = 0x10200000, // Input card buffer fixed address
  RIV_VADDR_OUTCARD      = 0x10400000, // Output card buffer fixed address
  RIV_VADDR_STATECARD    = 0x10600000, // State card buffer fixed address
  RIV_VADDR_FRAMEBUFFER  = 0x10800000, // Frame buffer fixed address
  RIV_VADDR_TXBUFFER     = 0x10a00000, // Transfer buffer fixed address
} riv_mem_addr;

// Canaries
typedef enum riv_canary_ids {
  RIV_CANARY_DRIVER = 0x3fdf371e,
  RIV_CANARY_DEVICE = 0x630b3ad1,
} riv_canary_ids;

////////////////////////////////////////////////////////////////////////////////
// Primitive types

typedef uint8_t* riv_bytes;

////////////////////////////////////////////////////////////////////////////////
// Structures

// Bounded contiguous array of bytes
typedef struct riv_span_uint8 {
  riv_bytes data;
  uint64_t size;
} riv_span_uint8;

// Vector2 (int64)
typedef struct riv_vec2i {
  int64_t x;
  int64_t y;
} riv_vec2i;

// Vector2 (float64)
typedef struct riv_vec2f {
  double x;
  double y;
} riv_vec2f;

// Rectangle (int64)
typedef struct riv_recti {
  int64_t x;
  int64_t y;
  int64_t width;
  int64_t height;
} riv_recti;

// Rectangle (float64)
typedef struct riv_rectf {
  double x;
  double y;
  double width;
  double height;
} riv_rectf;

// Draw image
typedef struct riv_image {
  riv_bytes pixels;           // Image pixel data, 1 byte per pixel
  uint16_t width;             // Image width
  uint16_t height;            // Image height
  int16_t color_key;          // Color on the image used to represent transparent pixels
  bool owned;                 // Whether pixel data should be freed when destroying the image
} riv_image;

// Draw sprite
typedef struct riv_spritesheet {
  uint64_t image_id;          // Image for the sprite sheet
  uint32_t cell_width;        // Common sprite width
  uint32_t cell_height;       // Common sprite height
} riv_spritesheet;

// Draw state
typedef struct riv_draw_state {
  riv_vec2i origin;            // Draw origin
  riv_recti clip;              // Draw clipping bounding box
  uint8_t pal[RIV_MAX_COLORS]; // Draw swap palette
  bool pal_enabled;            // Whether swap palette is enabled
  bool color_key_disabled;     // Whether drawing of sprites/images should consider color key
} riv_draw_state;

// Sound buffer description
typedef struct riv_soundbuffer_desc {
  uint64_t id;                 // Sound buffer id (filled automatically)
  riv_sound_format format;     // Sound format
  uint32_t channels;           // Sound channels (0 = auto detect)
  uint32_t sample_rate;        // Sound sample rate (0 = auto detect)
  riv_span_uint8 data;         // Sound data
} riv_soundbuffer_desc;

// Sound description
typedef struct riv_sound_desc {
  uint64_t id;                 // Sound id (filled automatically, or used when updating a sound)
  uint64_t buffer_id;          // Sound buffer id (must be 0 when updating a sound)
  float delay;                 // Start delay time in seconds (0 = no delay)
  float duration;              // Duration in seconds (0 = let id end, -1 = loop)
  float fade_in;               // Fade in time in seconds (0 = no fade in)
  float fade_out;              // Fade out time in seconds (0 = no fade out)
  float seek;                  // Seek in time in seconds (0 = no seek, -1 = stop)
  float volume;                // Volume gain in range [0.0, 1.0]
  float pan;                   // Pan in range [-1.0, 1.0]
  float pitch;                 // Pitch (0.0 = no pitch change)
} riv_sound_desc;

// Waveform sound description
typedef struct riv_waveform_desc {
  uint64_t id;                 // Sound id (filled automatically)
  riv_waveform_type type;      // Waveform type
  float delay;                 // Start delay in seconds
  float attack;                // Attack duration in seconds
  float decay;                 // Decay duration in seconds
  float sustain;               // Sustain duration in seconds
  float release;               // Release duration in seconds
  float start_frequency;       // Starting frequency in Hz
  float end_frequency;         // Starting frequency in Hz
  float amplitude;             // Maximum amplitude in range (0.0, 1.0]
  float sustain_level;         // Sustain level in range (0.0, 1.0]
  float duty_cycle;            // Duty cycle in range (0.0, 1.0]
  float pan;                   // Pan in range [-1.0, 1.0]
} riv_waveform_desc;

// Key state
typedef struct riv_key_state {
  uint64_t down_frame;         // Last frame the key was pressed
  uint64_t up_frame;           // Last frame the key was released
  bool down;                   // Becomes true when the key is pressed, becomes false when released
  bool up;                     // Becomes true when the key is released, becomes false when pressed
  bool press;                  // True only in the frame the key is pressed
  bool release;                // True only in the frame the key is released
} riv_key_state;

// Key toggle event
typedef struct riv_key_toggle_event {
  uint8_t key_code;     // Key that is changing its state
  uint64_t frame;       // Frame number that triggered the key toggle event
} riv_key_toggle_event;

// Pseudo random number generator based on xoshiro256
typedef struct riv_xoshiro256 {
  uint64_t state[4];
} riv_xoshiro256;

////////////////////////////////////////////////////////////////////////////////
// Structures used internally for device and driver communication

// Device audio command description
typedef union riv_command_desc {
  riv_soundbuffer_desc soundbuffer;
  riv_sound_desc sound;
  riv_waveform_desc waveform;
} riv_command_desc;

// Device audio command
typedef struct riv_command {
  riv_command_type type;
  riv_command_desc desc;
} riv_command;

// Memory mapped header
typedef struct riv_mmio_header {
  uint32_t canary;
  uint32_t version;
  uint32_t features;
  uint32_t uuid;
} riv_mmio_header;

// Driver memory mapped structure (driver writes, device reads)
typedef struct riv_mmio_driver {
  riv_mmio_header header;
  uint64_t frame;
  uint32_t outcard_len;
  uint32_t statecard_len;
  uint32_t height;
  uint32_t width;
  uint32_t target_fps;
  riv_pixel_format pixel_format;
  uint32_t palette[RIV_MAX_COLORS];
  bool tracked_keys[RIV_NUM_KEYCODE];
  riv_command commands[RIV_MAX_COMMANDS];
  uint32_t command_len;
} riv_mmio_driver;

// Device memory mapped structure (device writes, driver reads)
typedef struct riv_mmio_device {
  riv_mmio_header header;
  uint32_t incard_len;
  uint32_t initial_statecard_len;
  uint32_t key_toggle_count;
  uint8_t key_toggles[RIV_MAX_KEY_TOGGLES];
} riv_mmio_device;

////////////////////////////////////////////////////////////////////////////////
// RIV context structure
//
// The RIV context is a 12MB structure mapped at the virtual memory address 0x10000000,
// it contains all the public and private state used by other RIV APIs.
// Some fields can be read/write directly as convenience.
// Every field that can be read or written directly are marked as:
// - [R] - Can be read at any moment
// - [W] - Can be write at any moment
// - [RW] - Can be read/write at any moment
// Other fields without RW marks are used internally and should not be read or written.
//
typedef struct riv_context {
  //////////////////////////////////////
  // MMIO driver (driver writes/device reads)
  riv_mmio_header mmio_driver_header;       // MMIO driver header
  uint64_t frame;                           // [R] Current frame number
  uint32_t outcard_len;                     // [RW] Output card length
  uint32_t statecard_len;                   // [RW] State card length
  uint32_t height;                          // [RW] Screen height
  uint32_t width;                           // [RW] Screen width
  uint32_t target_fps;                      // [RW] Screen target refresh rate
  riv_pixel_format pixel_format;            // [RW] Screen pixel format
  uint32_t palette[RIV_MAX_COLORS];         // [RW] Color palette
  bool tracked_keys[RIV_NUM_KEYCODE];       // [RW] Key codes being tracked
  riv_command commands[RIV_MAX_COMMANDS];   // Command queue to be executed by the device
  uint32_t command_len;                     // Command queue length
  uint8_t mmio_driver_padding[780];         // Reserved

  //////////////////////////////////////
  // MMIO device (device writes/driver reads)
  riv_mmio_header mmio_device_header;       // MMIO device header
  uint32_t incard_len;                      // [R] Input card length
  uint32_t initial_statecard_len;           // [R] Initial state card length
  uint32_t key_toggle_count;                // [R] Number of toggled keys in this frame
  uint8_t key_toggles[RIV_MAX_KEY_TOGGLES]; // [R] Toggled key in this frame (in order)
  uint8_t mmio_device_padding[4004];        // Reserved

  //////////////////////////////////////
  // Public state
  int64_t time_ms;                            // [R] Current time in milliseconds since first frame
  double time;                                // [R] Current time in seconds since first frame
  uint64_t quit_frame;                        // [RW] Stop main loop at the specified frame
  bool quit;                                  // [RW] Whether to stop main loop after this frame
  bool valid;                                 // [R] Whether we are initialized
  bool verifying;                             // [R] Whether we are verifying
  bool yielding;                              // [R] Whether every frame should yield audio/video
  uint32_t key_modifiers;                     // [R] Current keyboard modifiers (e.g CTRL/ALT/SHIFT)
  riv_key_state keys[RIV_NUM_KEYCODE];        // [R] Current keyboard state
  riv_draw_state draw;                        // [RW] Draw state
  riv_image images[RIV_MAX_IMAGES];           // [RW] Loaded images
  riv_spritesheet spritesheets[RIV_MAX_SPRITESHEETS]; // [RW] Loaded sprite sheets
  uint8_t pub_driver_padding[904360];         // Reserved

  //////////////////////////////////////
  // Internal state
  riv_xoshiro256 prng;                        // PRNG state
  uint64_t entropy[128];                      // Entropy accumulated from inputs
  uint32_t entropy_index;                     // Entropy index
  uint32_t entropy_size;                      // Entropy size
  uint64_t sound_gen;                         // Counter for generating sound ids
  uint64_t soundbuffer_gen;                   // Counter for generating sound buffer ids
  uint64_t image_gen;                         // Counter for generating image ids
  uint64_t sprite_gen;                        // Counter for generating sprite ids
  uint32_t txbuffer_off;                      // Current transfer buffer offset
  uint32_t verify_key_event_index;            // Current event index when verifying
  riv_key_toggle_event *verify_key_events;    // List of events to follow when verifying
  uint64_t stop_frame;                        // Device requested stop frame
  uint64_t temp_str_off;                      // Temporary string buffer offset
  uint8_t temp_str_buf[RIV_SIZE_TEMP_STRBUF]; // Temporary string buffer (used by riv_tprintf)
  uint8_t prv_driver_padding[981912];         // Reserved

  //////////////////////////////////////
  // Buffers
  uint8_t incard[RIV_SIZE_INCARD];                    // [R] Input card buffer
  uint8_t outcard[RIV_SIZE_OUTCARD];                  // [RW] Output card buffer
  uint8_t statecard[RIV_SIZE_STATECARD];              // [RW] State card buffer
  uint8_t framebuffer[RIV_SIZE_FRAMEBUFFER];          // [RW] Screen frame buffer
  uint8_t txbuffer[RIV_SIZE_TXBUFFER];                // Data buffer for device commands
} riv_context;

////////////////////////////////////////////////////////////////////////////////
// RIV API

// Global RIV context
#ifdef RIV_IMPORT_CONTEXT
// RIV context exported as a symbol to be used with C FFI libraries
RIV_API riv_context *riv;
#else
// RIV context mapped at fixed virtual memory address 0x10000000
static riv_context *const riv = (riv_context*)RIV_VADDR_CONTEXT;
#endif

////////////////////////////////////////
// Basic

// Returns the RIV context, intended to be used with C FFI libraries
RIV_API riv_context* riv_get_context(void);
// Present current frame, returns true until quit is requested.
RIV_API bool riv_present(void);

////////////////////////////////////////
// Resources

// Load color palette from a PNG file
RIV_API uint32_t riv_load_palette(const char* filename, uint8_t start_col);
// Load an image from a PNG file
RIV_API uint64_t riv_make_image(const char* filename, int64_t color_key);
// Destroy an image
RIV_API void riv_destroy_image(uint64_t img_id);
// Create an sprite sheet from an image
RIV_API uint64_t riv_make_spritesheet(uint64_t img_id, uint32_t w, uint32_t h);
// Destroy an sprite sheet
RIV_API void riv_destroy_spritesheet(uint64_t sps_id);

////////////////////////////////////////
// Drawing

// Clear the screen
RIV_API void riv_clear(uint32_t col);
// Draw a point
RIV_API void riv_draw_point(int64_t x, int64_t y, uint32_t col);
// Draw a line determined by two points
RIV_API void riv_draw_line(int64_t x0, int64_t y0, int64_t x1, int64_t y1, uint32_t col);
// Draw fill of a rectangle determined by its top left and size
RIV_API void riv_draw_rect_fill(int64_t x0, int64_t y0, int64_t w, int64_t h, uint32_t col);
// Draw lines of a rectangle determined by its top left and size
RIV_API void riv_draw_rect_line(int64_t x0, int64_t y0, int64_t w, int64_t h, uint32_t col);
// Draw fill of a quad determined by 4 points
RIV_API void riv_draw_quad_fill(int64_t x0, int64_t y0, int64_t x1, int64_t y1, int64_t x2, int64_t y2, int64_t x3, int64_t y3, uint32_t col);
// Draw lines of a quad determined by 4 points
RIV_API void riv_draw_quad_line(int64_t x0, int64_t y0, int64_t x1, int64_t y1, int64_t x2, int64_t y2, int64_t x3, int64_t y3, uint32_t col);
// Draw fill of a rotated rectangle determined by its center, size and angle
RIV_API void riv_draw_box_fill(int64_t ox, int64_t oy, int64_t w, int64_t h, float rot, uint32_t col);
// Draw lines of a rotated rectangle determined by its center, size and angle
RIV_API void riv_draw_box_line(int64_t ox, int64_t oy, int64_t w, int64_t h, float rot, uint32_t col);
// Draw fill of a circle determined by its center and size
RIV_API void riv_draw_circle_fill(int64_t ox, int64_t oy, int64_t d, uint32_t col);
// Draw line of a circle determined by its center and size
RIV_API void riv_draw_circle_line(int64_t ox, int64_t oy, int64_t d, uint32_t col);
// Draw fill of a ellipse determined by its center and size
RIV_API void riv_draw_ellipse_fill(int64_t ox, int64_t oy, int64_t w, int64_t h, uint32_t col);
// Draw line of a ellipse determined by its center and size
RIV_API void riv_draw_ellipse_line(int64_t ox, int64_t oy, int64_t w, int64_t h, uint32_t col);
// Draw fill of a triangle determined by 3 points
RIV_API void riv_draw_triangle_fill(int64_t x0, int64_t y0, int64_t x1, int64_t y1, int64_t x2, int64_t y2, uint32_t col);
// Draw line of a triangle determined by 3 points
RIV_API void riv_draw_triangle_line(int64_t x0, int64_t y0, int64_t x1, int64_t y1, int64_t x2, int64_t y2, uint32_t col);
// Draw fill of a rectangle copied from an image
RIV_API void riv_draw_image_rect(uint64_t img_id, int64_t x0, int64_t y0, int64_t w, int64_t h, int64_t sx0, int64_t sy0, int64_t mw, int64_t mh);
// Draw fill of a rectangle copied from a sprite sheet
RIV_API void riv_draw_sprite(uint32_t n, uint64_t sps_id, int64_t x0, int64_t y0, int64_t nw, int64_t nh, int64_t mw, int64_t mh);
// Draw text determined by its sprite sheet, anchor point, size and returns its size
RIV_API riv_recti riv_draw_text(const char* text, uint64_t sps_id, riv_align anchor, int64_t x, int64_t y, int64_t size, int64_t col);
// Draw text determined by its sprite sheet, anchor point, size, scale, spacing and returns its size
RIV_API riv_recti riv_draw_text_ex(const char* text, uint64_t sps_id, riv_align anchor, int64_t x, int64_t y, int64_t mw, int64_t mh, int64_t sx, int64_t sy, int64_t col);

////////////////////////////////////////
// Audio

// Create a new sound buffer
RIV_API uint64_t riv_make_soundbuffer(riv_soundbuffer_desc* desc);
// Destroy a sound buffer
RIV_API void riv_destroy_soundbuffer(uint64_t sndbuf_id);
// Play a sound buffer or update a sound
RIV_API uint64_t riv_sound(riv_sound_desc* desc);
// Play a waveform sound
RIV_API uint64_t riv_waveform(riv_waveform_desc* desc);

////////////////////////////////////////
// Pseudo random number generator (PRNG)

// Seed pseudo random number generator
RIV_API void riv_srand(uint64_t a, uint64_t b);
// Return a random  uint64 in range [0, MAX_UINT64]
RIV_API uint64_t riv_rand(void);
// Return a random  uint64 in range [0, high]
RIV_API uint64_t riv_rand_uint(uint64_t high);
// Return a random   int64 in range [low, high]
RIV_API int64_t riv_rand_int(int64_t low, int64_t high);
// Return a random float64 in range [0.0, 1.0)
RIV_API double riv_rand_float(void);

////////////////////////////////////////
// Utilities

// Print to standard output, use for debugging only
RIV_API uint64_t riv_printf(const char* format, ...);
// Format a string into a temporary buffer, the string remains valid until next frame
RIV_API char *riv_tprintf(const char* format, ...);
// Format a string
RIV_API uint64_t riv_snprintf(char* buf, uint64_t n, const char* format, ...);
// Get the RIV version at runtime
RIV_API uint64_t riv_version(void);
// Get the current machine cycle, THIS IS NON REPRODUCIBLE, use for bench-marking only
RIV_API uint64_t riv_rdcycle(void);

#endif
