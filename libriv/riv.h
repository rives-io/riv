#ifndef RIV_H
#define RIV_H

#include <stdint.h>
#include <stdbool.h>

#define RIV_VERSION_MAJOR 0
#define RIV_VERSION_MINOR 2
#define RIV_VERSION_PATCH 0
#define RIV_VERSION (RIV_VERSION_MAJOR*1000000 + RIV_VERSION_MINOR*1000 + RIV_VERSION_PATCH)

#ifndef RIV_API
#define RIV_API
#endif

////////////////////////////////////////////////////////////////////////////////
// Constants

// All supported RIV key codes, specially mapped to fit in 7 bits
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
  RIV_KEYCODE_RIGHT               = 102,
  RIV_KEYCODE_LEFT                = 103,
  RIV_KEYCODE_DOWN                = 104,
  RIV_KEYCODE_UP                  = 105,
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
  RIV_NUM_KEYCODE                 = 128,
} riv_key_code;

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
  RIV_PIXELFORMAT_INVALID = 0,
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
  RIV_MAX_IMAGES = 256,
  RIV_MAX_SPRITES = 256,
  RIV_INVALID_ID = 0,
} riv_constants;

// Predefined image ids
typedef enum riv_image_id {
  RIV_IMAGE_NONE = 0,
  RIV_IMAGE_FONT_5X7 = RIV_MAX_SPRITES-1,
  RIV_IMAGE_FONT_3X5 = RIV_MAX_SPRITES-2,
} riv_image_id;

// Predefined sprite ids
typedef enum riv_sprite_id {
  RIV_SPRITE_NONE = 0,
  RIV_SPRITE_FONT_5X7 = RIV_MAX_IMAGES-1,
  RIV_SPRITE_FONT_3X5 = RIV_MAX_IMAGES-2,
} riv_sprite_id;

////////////////////////////////////////////////////////////////////////////////
// Constants used to implement the driver and device communication

// Device control commands
typedef enum riv_control_command {
  RIV_CONTROL_NONE = 0,
  RIV_CONTROL_OPEN,
  RIV_CONTROL_CLOSE,
  RIV_CONTROL_PRESENT,
  RIV_CONTROL_FLUSH_AUDIO,
} riv_control_command;

// Device audio commands
typedef enum riv_audio_command_type {
  RIV_AUDIOCOMMAND_NONE = 0,
  RIV_AUDIOCOMMAND_MAKE_SOUND_BUFFER,
  RIV_AUDIOCOMMAND_DESTROY_SOUND_BUFFER,
  RIV_AUDIOCOMMAND_SOUND,
  RIV_AUDIOCOMMAND_WAVEFORM,
} riv_audio_command_type;

// Memory mapped sizes
typedef enum riv_mem_size {
  RIV_MMIOSIZE_HUGEPAGE     = 2*1024*1024, // 2 MB
  RIV_MMIOSIZE_MMIO_DRIVER  =     64*1024, // 64 KB
  RIV_MMIOSIZE_MMIO_DEVICE  =     64*1024, // 64 KB
  RIV_MMIOSIZE_INOUTBUFFER  =    256*1024, // 256 KB
  RIV_MMIOSIZE_AUDIOBUFFER  =    768*1024, // 768 KB
  RIV_MMIOSIZE_FRAMEBUFFER  =   1024*1024, // 1 MB
} riv_mem_size;

// Memory mapped offsets
typedef enum riv_mmio_offset {
  RIV_MMIOSTART_MMIO_DRIVER  = 0,
  RIV_MMIOSTART_MMIO_DEVICE  = RIV_MMIOSTART_MMIO_DRIVER + RIV_MMIOSIZE_MMIO_DRIVER,
  RIV_MMIOSTART_INOUTBUFFER  = RIV_MMIOSTART_MMIO_DEVICE + RIV_MMIOSIZE_MMIO_DEVICE,
  RIV_MMIOSTART_AUDIOBUFFER  = RIV_MMIOSTART_INOUTBUFFER + RIV_MMIOSIZE_INOUTBUFFER,
  RIV_MMIOSTART_FRAMEBUFFER  = RIV_MMIOSTART_AUDIOBUFFER + RIV_MMIOSIZE_AUDIOBUFFER,
} riv_mmio_offset;

// Memory mapped virtual address bases
typedef enum riv_vaddr_base {
  RIV_VADDR_BASE         = 0x10000000,
  RIV_VADDR_MMIO_DRIVER  = RIV_VADDR_BASE + RIV_MMIOSTART_MMIO_DRIVER,
  RIV_VADDR_MMIO_DEVICE  = RIV_VADDR_BASE + RIV_MMIOSTART_MMIO_DEVICE,
  RIV_VADDR_INOUTBUFFER  = RIV_VADDR_BASE + RIV_MMIOSTART_INOUTBUFFER,
  RIV_VADDR_AUDIOBUFFER  = RIV_VADDR_BASE + RIV_MMIOSTART_AUDIOBUFFER,
  RIV_VADDR_FRAMEBUFFER  = RIV_VADDR_BASE + RIV_MMIOSTART_FRAMEBUFFER,
} riv_vaddr_base;

// RIV driver magic when initializing
#define RIV_DRIVER_MAGIC (riv_magic_buffer){{ \
  0x3f, 0xdf, 0x37, 0x1e, 0xc0, 0xfc, 0xd1, 0xba, \
  0xec, 0xe9, 0x72, 0xa1, 0xf5, 0x89, 0x87, 0xc5, \
  0x70, 0xfd, 0xbe, 0xc0, 0xce, 0xcc, 0x2d, 0x74, \
  0x8d, 0x45, 0x39, 0x62, 0x49, 0xb8, 0x15, 0x26, \
}}

// RIV device magic when initializing
#define RIV_DEVICE_MAGIC (riv_magic_buffer){{ \
  0x83, 0x0b, 0x3a, 0xd1, 0xcc, 0x8b, 0xc2, 0xe5, \
  0x70, 0x5c, 0x83, 0x98, 0x6c, 0xe4, 0x67, 0xc9, \
  0xc1, 0xc6, 0x0b, 0xc6, 0xb9, 0x80, 0xa4, 0x1c, \
  0x34, 0x12, 0x8c, 0x2e, 0x05, 0xd8, 0x2c, 0x4e, \
}}

////////////////////////////////////////////////////////////////////////////////
// Primitive types

typedef uint8_t* riv_unbounded_uint8;
typedef uint32_t* riv_unbounded_uint32;
typedef bool* riv_unbounded_bool;
typedef uint64_t riv_id;

typedef struct riv_span_uint8 {
  riv_unbounded_uint8 data;
  uint64_t size;
} riv_span_uint8;

////////////////////////////////////////////////////////////////////////////////
// Structures used by RIV commands

// Frame buffer description
typedef struct riv_framebuffer_desc {
  uint32_t height;               // Screen height
  uint32_t width;                // Screen width
  uint32_t target_fps;           // Screen target refresh rate
  riv_pixel_format pixel_format; // Screen pixel format
} riv_framebuffer_desc;

// Sound buffer description
typedef struct riv_sound_buffer_desc {
  riv_id id;                // Sound buffer id (filled automatically)
  riv_sound_format format;  // Sound format
  uint32_t channels;        // Sound channels (0 = auto detect)
  uint32_t sample_rate;     // Sound sample rate (0 = auto detect)
  riv_span_uint8 data;      // Sound data
} riv_sound_buffer_desc;

// Sound description
typedef struct riv_sound_desc {
  riv_id id;          // Sound id (filled automatically, or used when updating a sound)
  riv_id buffer_id;   // Sound buffer id (must be 0 when updating a sound)
  float delay;        // Start delay time in seconds (0 = no delay)
  float duration;     // Duration in seconds (0 = let id end, -1 = loop)
  float fade_in;      // Fade in time in seconds (0 = no fade in)
  float fade_out;     // Fade out time in seconds (0 = no fade out)
  float seek;         // Seek in time in seconds (0 = no seek, -1 = stop)
  float volume;       // Volume gain in range [0.0, 1.0]
  float pan;          // Pan in range [-1.0, 1.0]
  float pitch;        // Pitch (0.0 = no pitch change)
} riv_sound_desc;

// Waveform sound description
typedef struct riv_waveform_desc {
  riv_id id;              // Sound id (filled automatically)
  riv_waveform_type type; // Waveform type
  float delay;            // Start delay in seconds
  float attack;           // Attack duration in seconds
  float decay;            // Decay duration in seconds
  float sustain;          // Sustain duration in seconds
  float release;          // Release duration in seconds
  float start_frequency;  // Starting frequency in Hz
  float end_frequency;    // Starting frequency in Hz
  float amplitude;        // Maximum amplitude in range [0.0, 1.0]
  float sustain_level;    // Sustain level in range [0.0, 1.0]
  float duty_cycle;       // Duty cycle in range [0.0, 1.0]
  float pan;              // Pan in range [-1.0, 1.0]
} riv_waveform_desc;

////////////////////////////////////////////////////////////////////////////////
// Structures used for device and driver communication

// Device audio command description
typedef union riv_audio_command_desc {
  riv_sound_buffer_desc sound_buffer;
  riv_sound_desc sound;
  riv_waveform_desc waveform;
} riv_audio_command_desc;

// Device audio command
typedef struct riv_audio_command {
  riv_audio_command_type type;
  riv_audio_command_desc desc;
} riv_audio_command;

// Magic buffer
typedef struct riv_magic_buffer {
  uint8_t data[32];
} riv_magic_buffer;

// Memory mapped header
typedef struct riv_mmio_header {
  riv_magic_buffer magic;
  uint32_t version;
  uint32_t features;
  uint64_t uuid;
} riv_mmio_header;

// Driver memory mapped structure (driver writes, device reads)
typedef struct riv_mmio_driver {
  riv_mmio_header header;
  uint64_t frame;
  riv_framebuffer_desc framebuffer_desc;
  riv_audio_command audio_commands[32];
  uint32_t audio_command_len;
  uint32_t outcard_len;
  uint32_t palette[RIV_MAX_COLORS];
  bool tracked_keys[RIV_NUM_KEYCODE];
} riv_mmio_driver;

// Device memory mapped structure (device writes, driver reads)
typedef struct riv_mmio_device {
  riv_mmio_header header;
  uint32_t incard_len;
  uint32_t key_toggle_count;
  uint8_t key_toggles[RIV_NUM_KEYCODE];
} riv_mmio_device;

////////////////////////////////////////////////////////////////////////////////
// Structures used only in the driver

// Pseudo random number generator based on xoshiro256
typedef struct riv_xoshiro256 {
  uint64_t state[4];
} riv_xoshiro256;

// Key state
typedef struct riv_key_state {
  uint64_t down_frame;  // Last frame the key was pressed
  uint64_t up_frame;    // Last frame the key was released
  bool down;            // Becomes trues when the key is pressed, becomes false only when it is released
  bool up;              // Becomes trues when the key is released, becomes false only when it is pressed
  bool press;           // True only in the frame the key is pressed
  bool release;         // True only in the frame the key is released
} riv_key_state;

// Key toggle event
typedef struct riv_key_toggle_event {
  uint8_t key_code;
  uint64_t frame;
} riv_key_toggle_event;

// Vector 2 of integer, can represent points or sizes
typedef struct riv_vec2i {
  int64_t x;
  int64_t y;
} riv_vec2i;

// Vector 2 of floats, can represent points or sizes
typedef struct riv_vec2f {
  float x;
  float y;
} riv_vec2f;

// Bounding box
typedef struct riv_bboxi {
  int64_t x0;
  int64_t y0;
  int64_t x1;
  int64_t y1;
} riv_bboxi;

// Draw image
typedef struct riv_image {
  riv_unbounded_uint8 pixels;
  uint16_t width;
  uint16_t height;
  int16_t color_key;
  bool owned;
} riv_image;

// Draw sprite
typedef struct riv_sprite {
  riv_id image_id;
  uint32_t grid_width;
  uint32_t grid_height;
} riv_sprite;

// Draw state
typedef struct riv_draw_state {
  riv_vec2i origin;                 // Draw origin
  riv_bboxi clip;                   // Draw clipping bounding box
  uint8_t pal[RIV_MAX_COLORS];      // Draw swap palette
} riv_draw_state;

// RIV context
typedef struct riv_context {
  // Public read-only fields (can be read at any moment)
  uint64_t frame;                         // Current frame number
  int64_t millis;                         // Current time in microseconds since first frame
  double seconds;                         // Current time in seconds since first frame
  uint32_t incard_len;                    // Input card length
  bool valid;                             // Whether riv is initialized
  bool verifying;                         // Whether we are verifying
  bool yielding;                          // Whether an audio/video/input devices are connected and yielding
  uint32_t key_modifiers;                 // NIY
  uint32_t key_toggle_count;              // Number of toggled keys in this frame
  uint8_t key_toggles[RIV_NUM_KEYCODE];   // Toggled key in this frame (in order)
  riv_key_state keys[RIV_NUM_KEYCODE];    // Current keyboard state
  int8_t reserved_r[856];
  // Public read/write fields (can be written at any moment)
  uint32_t outcard_len;                   // Output card length
  bool quit;                              // When set true, RIV loop will stop
  riv_framebuffer_desc* framebuffer_desc; // Screen frame buffer description
  riv_unbounded_bool tracked_keys;        // Key codes being tracked
  riv_unbounded_uint8 inoutbuffer;        // Input/output card buffer
  riv_unbounded_uint32 palette;           // Color palette
  riv_unbounded_uint8 framebuffer;        // Screen frame buffer
  riv_draw_state draw;                    // Draw state
  riv_image images[RIV_MAX_IMAGES];       // All loaded images
  riv_sprite sprites[RIV_MAX_SPRITES];      // All loaded sprites
  uint8_t reserved_rw[168];
  // Private fields
  riv_xoshiro256 prng;                    // Internal PRNG state
  riv_mmio_driver* mmio_driver;
  riv_mmio_device* mmio_device;
  riv_unbounded_uint8 audiobuffer;        // Audio buffer used by audio commands
  uint64_t entropy[128];
  uint32_t entropy_index;
  uint32_t entropy_size;
  uint64_t sound_gen;                     // Counter for generating sound ids
  uint64_t sound_buffer_gen;              // Counter for generating sound buffer ids
  uint64_t image_gen;                     // Counter for generating image ids
  uint64_t sprite_gen;                    // Counter for generating sprite ids
  uint32_t audiobuffer_off;
  int32_t yield_fd;
  int32_t verify_iocard_fd;
  uint32_t verify_key_event_index;
  riv_key_toggle_event *verify_key_events;
  uint64_t stop_frame;
} riv_context;

////////////////////////////////////////////////////////////////////////////////
// RIV API

extern riv_context riv;

// Utilities

RIV_API uint64_t riv_version(void);                                               // Get the RIV runtime version
RIV_API uint64_t riv_rdcycle(void);                                               // Get the current machine cycle, THIS IS NOT REPRODUCIBLE, use for bench-marking only
RIV_API uint64_t riv_printf(const char* format, ...);                             // Print to standard output, use for debugging
RIV_API uint64_t riv_snprintf(char* s, uint64_t maxlen, const char* format, ...); // Print to standard output, use for debugging

// Basic

RIV_API void riv_setup(int32_t argc, char** argv);    // Initialize RIV driver
RIV_API void riv_shutdown();                          // Terminate RIV driver
RIV_API bool riv_present();                           // Present current frame, returns true when quit was requested in the frame, false otherwise.

// Images

RIV_API riv_id riv_make_image(const char* filename, int64_t color_key);
RIV_API void riv_destroy_image(riv_id img);

// Sprites

RIV_API riv_id riv_make_sprite(riv_id img_id, uint32_t w, uint32_t h);
RIV_API void riv_destroy_sprite(riv_id spr_id);

// Drawing

RIV_API void riv_clear_screen(uint32_t col);
RIV_API void riv_draw_point(int64_t x, int64_t y, uint32_t col);
RIV_API void riv_draw_line(int64_t x0, int64_t y0, int64_t x1, int64_t y1, uint32_t col);
RIV_API void riv_draw_rect_fill(int64_t x0, int64_t y0, int64_t w, int64_t h, uint32_t col);
RIV_API void riv_draw_rect_line(int64_t x0, int64_t y0, int64_t w, int64_t h, uint32_t col);
RIV_API void riv_draw_quad_fill(int64_t x0, int64_t y0, int64_t x1, int64_t y1, int64_t x2, int64_t y2, int64_t x3, int64_t y3, uint32_t col);
RIV_API void riv_draw_quad_line(int64_t x0, int64_t y0, int64_t x1, int64_t y1, int64_t x2, int64_t y2, int64_t x3, int64_t y3, uint32_t col);
RIV_API void riv_draw_box_fill(int64_t ox, int64_t oy, int64_t w, int64_t h, float rot, uint32_t col);
RIV_API void riv_draw_box_line(int64_t ox, int64_t oy, int64_t w, int64_t h, float rot, uint32_t col);
RIV_API void riv_draw_circle_fill(int64_t ox, int64_t oy, int64_t d, uint32_t col);
RIV_API void riv_draw_circle_line(int64_t ox, int64_t oy, int64_t d, uint32_t col);
RIV_API void riv_draw_ellipse_fill(int64_t ox, int64_t oy, int64_t w, int64_t h, uint32_t col);
RIV_API void riv_draw_ellipse_line(int64_t ox, int64_t oy, int64_t w, int64_t h, uint32_t col);
RIV_API void riv_draw_triangle_fill(int64_t x0, int64_t y0, int64_t x1, int64_t y1, int64_t x2, int64_t y2, uint32_t col);
RIV_API void riv_draw_triangle_line(int64_t x0, int64_t y0, int64_t x1, int64_t y1, int64_t x2, int64_t y2, uint32_t col);
RIV_API void riv_draw_image_rect(riv_id img_id, int64_t x0, int64_t y0, int64_t w, int64_t h, int64_t sx0, int64_t sy0, int64_t mw, int64_t mh);
RIV_API void riv_draw_sprite(uint32_t n, riv_id spr_id, int64_t x0, int64_t y0, int64_t nw, int64_t nh, int64_t mw, int64_t mh);
RIV_API riv_vec2i riv_draw_text(const char* text, riv_id spr_id, int64_t x0, int64_t y0, int64_t col, int64_t mw, int64_t mh, int64_t sx, int64_t sy);

// Sound system

RIV_API uint64_t riv_make_sound_buffer(riv_sound_buffer_desc* desc); // Create a new sound buffer
RIV_API void riv_destroy_sound_buffer(riv_id sndbuf_id);             // Destroy a sound buffer
RIV_API uint64_t riv_sound(riv_sound_desc* desc);                    // Play a sound buffer or update a sound
RIV_API uint64_t riv_waveform(riv_waveform_desc* desc);              // Play a waveform sound

// Pseudo random number generator (PRNG)

RIV_API void riv_srand(uint64_t a, uint64_t b);          // Seed PRNG
RIV_API uint64_t riv_rand();                             // Returns a random uint64 in range [0, MAX_UINT64]
RIV_API uint64_t riv_rand_uint(uint64_t high);           // Returns a random uint64 in range [0, high]
RIV_API int64_t riv_rand_int(int64_t low, int64_t high); // Returns a random  int64 in range [low, high]
RIV_API double riv_rand_float();                         // Returns a random float64 in range [0.0, 1.0)

#endif
