#ifndef RIV_H
#define RIV_H

#include <stdint.h>
#include <stdbool.h>

////////////////////////////////////////////////////////////////////////////////
// Defines

#define RIV_VERSION_MAJOR 0
#define RIV_VERSION_MINOR 1
#define RIV_VERSION_PATCH 0
#define RIV_VERSION (RIV_VERSION_MAJOR*1000000 + RIV_VERSION_MINOR*1000 + RIV_VERSION_PATCH)

#define RIV_DRIVER_MAGIC { \
  0x3f, 0xdf, 0x37, 0x1e, 0xc0, 0xfc, 0xd1, 0xba, \
  0xec, 0xe9, 0x72, 0xa1, 0xf5, 0x89, 0x87, 0xc5, \
  0x70, 0xfd, 0xbe, 0xc0, 0xce, 0xcc, 0x2d, 0x74, \
  0x8d, 0x45, 0x39, 0x62, 0x49, 0xb8, 0x15, 0x26, \
}

#define RIV_DEVICE_MAGIC { \
  0x83, 0x0b, 0x3a, 0xd1, 0xcc, 0x8b, 0xc2, 0xe5, \
  0x70, 0x5c, 0x83, 0x98, 0x6c, 0xe4, 0x67, 0xc9, \
  0xc1, 0xc6, 0x0b, 0xc6, 0xb9, 0x80, 0xa4, 0x1c, \
  0x34, 0x12, 0x8c, 0x2e, 0x05, 0xd8, 0x2c, 0x4e, \
}

#ifndef RIV_API
#define RIV_API
#endif

////////////////////////////////////////////////////////////////////////////////
// Constants

// All supported RIV key codes, specially mapped to fit in 7 bits.
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
  RIV_NUM_KEYCODE                 = 128
} riv_key_code;

// Default palette color indexes.
typedef enum riv_pal16_color {
  RIV_PAL16_BLACK      = 0,
  RIV_PAL16_DARKBLUE   = 1,
  RIV_PAL16_DARKPURPLE = 2,
  RIV_PAL16_DARKGREEN  = 3,
  RIV_PAL16_BROWN      = 4,
  RIV_PAL16_DARKGREY   = 5,
  RIV_PAL16_LIGHTGREY  = 6,
  RIV_PAL16_WHITE      = 7,
  RIV_PAL16_RED        = 8,
  RIV_PAL16_ORANGE     = 9,
  RIV_PAL16_YELLOW     = 10,
  RIV_PAL16_GREEN      = 11,
  RIV_PAL16_BLUE       = 12,
  RIV_PAL16_LAVENDER   = 13,
  RIV_PAL16_PINK       = 14,
  RIV_PAL16_LIGHTPEACH = 15,
  RIV_NUM_PAL16_COLOR  = 16
} riv_pal16_color;

// Default palette colors.
typedef enum riv_rgb_pal16_color {
  RIV_RGB_PAL16_BLACK      = 0x000000,
  RIV_RGB_PAL16_DARKBLUE   = 0x532B1D,
  RIV_RGB_PAL16_DARKPURPLE = 0x53257E,
  RIV_RGB_PAL16_DARKGREEN  = 0x518700,
  RIV_RGB_PAL16_BROWN      = 0x3652AB,
  RIV_RGB_PAL16_DARKGREY   = 0x4F575F,
  RIV_RGB_PAL16_LIGHTGREY  = 0xC7C3C2,
  RIV_RGB_PAL16_WHITE      = 0xE8F1FF,
  RIV_RGB_PAL16_RED        = 0x4D00FF,
  RIV_RGB_PAL16_ORANGE     = 0x00A3FF,
  RIV_RGB_PAL16_YELLOW     = 0x27ECFF,
  RIV_RGB_PAL16_GREEN      = 0x36E400,
  RIV_RGB_PAL16_BLUE       = 0xFFAD29,
  RIV_RGB_PAL16_LAVENDER   = 0x9C7683,
  RIV_RGB_PAL16_PINK       = 0xA877FF,
  RIV_RGB_PAL16_LIGHTPEACH = 0xAACCFF,
} riv_rgb_pal16_color;

typedef enum riv_pixel_format {
  RIV_PIXELFORMAT_INVALID = 0,
  RIV_PIXELFORMAT_PAL256,
  // RIV_PIXELFORMAT_RGB8,
  // RIV_PIXELFORMAT_RGB16,
  // RIV_PIXELFORMAT_RGB24,
  // RIV_PIXELFORMAT_RGB32,
  // RIV_PIXELFORMAT_RGB32F,
  RIV_NUM_PIXELFORMAT,
  RIV_DEFAULT_PIXELFORMAT = RIV_PIXELFORMAT_PAL256
} riv_pixel_format;

typedef enum riv_constants {
  RIV_DEFAULT_WIDTH = 256,
  RIV_DEFAULT_HEIGHT = 256,
  RIV_DEFAULT_FPS = 60,
} riv_constants;

typedef enum riv_card_format {
  RIV_CARDFORMAT_NONE = 0,
  RIV_CARDFORMAT_DATA,
  RIV_CARDFORMAT_TEXT,
  RIV_CARDFORMAT_JSON,
} riv_card_format;

typedef enum riv_waveform_type {
  RIV_WAVEFORM_NONE = 0,
  RIV_WAVEFORM_SINE,
  RIV_WAVEFORM_SQUARE,
  RIV_WAVEFORM_TRIANGLE,
  RIV_WAVEFORM_SAWTOOTH,
  RIV_WAVEFORM_NOISE,
  RIV_WAVEFORM_PULSE,
} riv_waveform_type;

typedef enum riv_sound_format {
  RIV_SOUNDFORMAT_NONE = 0,
  RIV_SOUNDFORMAT_U8,  // 8-bit unsigned integer                 [0, 255]
  RIV_SOUNDFORMAT_S16, // 16-bit signed integer                  [-32768, 32767]
  RIV_SOUNDFORMAT_S24, // 24-bit signed integer (tightly packed) [-8388608, 8388607]
  RIV_SOUNDFORMAT_S32, // 32-bit signed integer                  [-2147483648, 2147483647]
  RIV_SOUNDFORMAT_F32, // 32-bit floating point                  [-1, 1]
  RIV_SOUNDFORMAT_WAV,
  RIV_SOUNDFORMAT_FLAC,
  RIV_SOUNDFORMAT_MP3,
  RIV_SOUNDFORMAT_VORBIS,
} riv_sound_format;

// The next constants are used to implement the driver

typedef enum riv_control_reason {
  RIV_CONTROL_NONE = 0,
  RIV_CONTROL_OPEN,
  RIV_CONTROL_CLOSE,
  RIV_CONTROL_PRESENT,
  RIV_CONTROL_FLUSH_AUDIO,
} riv_control_reason;

typedef enum riv_audio_command_type {
  RIV_AUDIOCOMMAND_NONE = 0,
  RIV_AUDIOCOMMAND_MAKE_SOUND_BUFFER,
  RIV_AUDIOCOMMAND_DESTROY_SOUND_BUFFER,
  RIV_AUDIOCOMMAND_SOUND,
  RIV_AUDIOCOMMAND_WAVEFORM,
} riv_audio_command_type;

typedef enum riv_mem_size {
  RIV_MEMSIZE_HUGEPAGE     = 2*1024*1024, // 2 MB
  RIV_MEMSIZE_MMIO_DRIVER  =    128*1024, // 128 KB
  RIV_MEMSIZE_MMIO_DEVICE  =    128*1024, // 128 KB
  RIV_MEMSIZE_INOUTBUFFER  =    256*1024, // 256 KB
  RIV_MEMSIZE_AUDIOBUFFER  =    512*1024, // 512 KB
  RIV_MEMSIZE_FRAMEBUFFER  =   1024*1024, // 1 MB
} riv_mem_size;

typedef enum riv_mmio_offset {
  RIV_MMIOSTART_MMIO_DRIVER  = 0,
  RIV_MMIOSTART_MMIO_DEVICE  = RIV_MMIOSTART_MMIO_DRIVER + RIV_MEMSIZE_MMIO_DRIVER,
  RIV_MMIOSTART_INOUTBUFFER  = RIV_MMIOSTART_MMIO_DEVICE + RIV_MEMSIZE_MMIO_DEVICE,
  RIV_MMIOSTART_AUDIOBUFFER  = RIV_MMIOSTART_INOUTBUFFER + RIV_MEMSIZE_INOUTBUFFER,
  RIV_MMIOSTART_FRAMEBUFFER  = RIV_MMIOSTART_AUDIOBUFFER + RIV_MEMSIZE_AUDIOBUFFER,
} riv_mmio_offset;

typedef enum riv_vaddr_base {
  RIV_VADDR_BASE         = 0x10000000,
  RIV_VADDR_MMIO_DRIVER  = RIV_VADDR_BASE + RIV_MMIOSTART_MMIO_DRIVER,
  RIV_VADDR_MMIO_DEVICE  = RIV_VADDR_BASE + RIV_MMIOSTART_MMIO_DEVICE,
  RIV_VADDR_INOUTBUFFER  = RIV_VADDR_BASE + RIV_MMIOSTART_INOUTBUFFER,
  RIV_VADDR_AUDIOBUFFER  = RIV_VADDR_BASE + RIV_MMIOSTART_AUDIOBUFFER,
  RIV_VADDR_FRAMEBUFFER  = RIV_VADDR_BASE + RIV_MMIOSTART_FRAMEBUFFER,
} riv_vaddr_base;

////////////////////////////////////////////////////////////////////////////////
// Primitive types

typedef uint8_t* riv_unbounded_uint8;
typedef uint32_t* riv_unbounded_uint32;
typedef bool* riv_unbounded_bool;

typedef struct riv_span_uint8 {
  uint8_t* data;
  uint64_t size;
} riv_span_uint8;

////////////////////////////////////////////////////////////////////////////////
// Structures used by both device and driver

typedef struct riv_framebuffer_desc {
  uint32_t height;
  uint32_t width;
  uint32_t fps;
  riv_pixel_format pixel_format;
} riv_framebuffer_desc;

typedef struct riv_sound_buffer_desc {
  uint64_t id;              // Sound buffer id (filled automatically)
  riv_sound_format format;  // Sound format
  uint32_t channels;        // Sound channels (0 = auto detect)
  uint32_t sample_rate;     // Sound sample rate (0 = auto detect)
  riv_span_uint8 data;      // Sound data
} riv_sound_buffer_desc;

typedef struct riv_sound_desc {
  uint64_t id;        // Sound id (used only when updating a sound)
  uint64_t buffer_id; // Sound buffer id
  uint32_t delay;     // Start delay time in milliseconds (0 = no delay)
  uint32_t duration;  // Duration in milliseconds (0 = let id end, 0xffffffff = loop)
  uint32_t fade_in;   // Fade in time in milliseconds (0 = no fade in)
  uint32_t fade_out;  // Fade out time in milliseconds (0 = no fade out)
  uint32_t seek;      // Seek in time in milliseconds (0 = no seek, 0xfffffffe = rewind, 0xffffffff = stop)
  float volume;       // Volume gain in range 0.0 - 1.0
  float pan;          // Pan (-1.0 = pan left, 0.0 = no pan, 1.0 = pan right)
  float pitch;        // Pitch (0.0 = no pitch change)
} riv_sound_desc;

typedef struct riv_waveform_desc {
  uint64_t id;            // Sound id (filled automatically)
  riv_waveform_type type; // Waveform type
  float delay;            // Start delay in seconds
  float attack;           // Attack duration in seconds
  float decay;            // Decay duration in seconds
  float sustain;          // Sustain duration in seconds
  float release;          // Release duration in seconds
  float start_frequency;  // Starting frequency in Hz
  float end_frequency;    // Starting frequency in Hz
  float amplitude;        // Maximum amplitude, in range [0.0, 1.0]
  float sustain_level;    // Sustain level, in range [0.0, 1.0]
  float duty_cycle;       // Duty cycle, in range [0.0, 1.0]
  float pan;              // Panning, in range [-1.0, 1.0]
} riv_waveform_desc;

typedef union riv_audio_command_desc {
  riv_sound_buffer_desc sound_buffer;
  riv_sound_desc sound;
  riv_waveform_desc waveform;
} riv_audio_command_desc;

typedef struct riv_audio_command {
  riv_audio_command_type type;
  riv_audio_command_desc desc;
} riv_audio_command;

typedef struct riv_mmio_header {
  uint8_t magic[32];
  uint32_t version;
  uint32_t features;
  uint64_t uuid;
} riv_mmio_header;

// Only driver writes, only device reads.
typedef struct riv_mmio_driver {
  riv_mmio_header header;
  uint64_t frame;
  // Video
  riv_framebuffer_desc framebuffer_desc;
  // Audio
  riv_audio_command audio_commands[32];
  uint32_t audio_command_len;
  // Outcard
  riv_card_format outcard_format;
  uint32_t outcard_len;
  // Keyboard
  bool tracked_keys[RIV_NUM_KEYCODE];
  // Palette
  uint32_t palette[256];
} riv_mmio_driver;

// Only device writes, only driver reads.
typedef struct riv_mmio_device {
  riv_mmio_header header;
  uint32_t incard_len;
  riv_card_format incard_format;
  uint32_t key_toggle_count;
  uint8_t key_toggles[RIV_NUM_KEYCODE];
} riv_mmio_device;

////////////////////////////////////////////////////////////////////////////////
// Structures used only in the driver

// Pseudo random number generator.
typedef struct riv_prng {
  uint64_t state[4];
} riv_prng;

typedef struct riv_key_state {
  bool press;           // Becomes trues when the key is pressed, becomes false only when it is released
  bool down;            // True only in the frame the key is pressed
  bool up;              // True only in the frame the key is released
  uint64_t down_frame;  // Last frame the key was pressed
  uint64_t up_frame;    // Last frame the key was released
} riv_key_state;

typedef struct riv_context {
  // Public read-only fields
  riv_key_state keys[RIV_NUM_KEYCODE];
  uint32_t key_modifiers; // NIY
  uint64_t frame;
  uint32_t incard_len;
  riv_card_format incard_format;
  // Public read/write fields
  uint32_t outcard_len;
  riv_card_format outcard_format;
  bool quit;
  riv_prng prng;
  riv_framebuffer_desc* framebuffer_desc;
  riv_unbounded_uint8 inoutbuffer;
  riv_unbounded_uint8 framebuffer;
  riv_unbounded_uint8 audiobuffer;
  riv_unbounded_bool tracked_keys;
  riv_unbounded_uint32 palette;
  // Private fields
  riv_mmio_driver* mmio_driver;
  riv_mmio_device* mmio_device;
  int32_t yield_fd;
  uint32_t audiobuffer_off;
  uint64_t sound_gen;
  uint64_t sound_buffer_gen;
  uint32_t entropy_index;
  uint32_t entropy_size;
  uint64_t entropy[128];
} riv_context;

typedef void (*riv_context_callback)(riv_context*);

typedef struct riv_run_desc {
  riv_context_callback init_cb;
  riv_context_callback cleanup_cb;
  riv_context_callback frame_cb;
  riv_framebuffer_desc framebuffer_desc;
  int32_t argc;
  char** argv;
} riv_run_desc;

////////////////////////////////////////////////////////////////////////////////
// API

// Utilities
RIV_API uint64_t riv_version(void); // Get the RIV library version
RIV_API uint64_t riv_rdcycle(void); // Get the current machine cycle, THIS IS NOT REPRODUCIBLE, use for benchmarking only.
RIV_API uint64_t riv_printf(const char* format, ...); // Print to standard output, use for debugging.
RIV_API uint64_t riv_snprintf(char* s, uint64_t maxlen, const char* format, ...); // Print to standard output, use for debugging.

// Basic
RIV_API void riv_setup(riv_context* self, int32_t argc, char** argv);
RIV_API void riv_shutdown(riv_context* self);
RIV_API void riv_present(riv_context* self);
RIV_API void riv_loop(riv_context* self, riv_context_callback frame_cb);
RIV_API void riv_run(riv_run_desc* run_desc);

// Sound system
RIV_API uint64_t riv_make_sound_buffer(riv_context* self, riv_sound_buffer_desc desc);
RIV_API void riv_destroy_sound_buffer(riv_context* self, uint64_t id);
RIV_API uint64_t riv_sound(riv_context* self, riv_sound_desc desc);
RIV_API uint64_t riv_waveform(riv_context* self, riv_waveform_desc desc);

// Pseudo random number generator (PRNG)
RIV_API void riv_srand(riv_prng* self, uint64_t a, uint64_t b);
RIV_API uint64_t riv_rand(riv_prng* self);
RIV_API uint64_t riv_rand_uint(riv_prng* self, uint64_t high);
RIV_API int64_t riv_rand_int(riv_prng* self, int64_t low, int64_t high);
RIV_API double riv_rand_float(riv_prng* self);

#endif
