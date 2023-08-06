#ifndef RIV_H
#define RIV_H

#include <stdint.h>
#include <stdbool.h>

////////////////////////////////////////////////////////////////////////////////
// Defines

#define RIV_VERSION 1000

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
  RIV_KEYCODE_LEFT_CONTROL        = 115,
  RIV_KEYCODE_LEFT_ALT            = 116,
  RIV_KEYCODE_LEFT_SUPER          = 117,
  RIV_KEYCODE_RIGHT_SHIFT         = 118,
  RIV_KEYCODE_RIGHT_CONTROL       = 119,
  RIV_KEYCODE_RIGHT_ALT           = 120,
  RIV_KEYCODE_RIGHT_SUPER         = 121,
  RIV_KEYCODE_MENU                = 122,
  // ASCII 123 {
  // ASCII 124 |
  // ASCII 125 }
  // ASCII 126 ~
} riv_key_code;

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
} riv_pal16_color;

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

typedef enum riv_control_reason {
  RIV_CONTROL_UNKNOWN = 0,
  RIV_CONTROL_PRESENT,
  RIV_CONTROL_AUDIO,
} riv_control_reason;

typedef enum riv_pixel_format {
  RIV_PIXELFORMAT_UNKNOWN = 0,
  RIV_PIXELFORMAT_PAL256,
  // RIV_PIXELFORMAT_RGB8,
  // RIV_PIXELFORMAT_RGB16,
  // RIV_PIXELFORMAT_RGB24,
  // RIV_PIXELFORMAT_RGB32,
  // RIV_PIXELFORMAT_RGB32F,
  RIV_DEFAULT_PIXELFORMAT = RIV_PIXELFORMAT_PAL256
} riv_pixel_format;

typedef enum riv_audio_command {
  RIV_AUDIOCOMMAND_NONE = 0,
  RIV_AUDIOCOMMAND_SOUND_PLAY,
  RIV_AUDIOCOMMAND_SOUND_STOP,
} riv_audio_command;

typedef enum riv_mem_size {
  RIV_MEMSIZE_HUGEPAGE     =   2*1024*1024, // 2 MB
  RIV_MEMSIZE_MMIO_DRIVER  =  128*1024, // 128 KB
  RIV_MEMSIZE_MMIO_DEVICE  =  128*1024, // 128 KB
  RIV_MEMSIZE_AUDIOBUFFER  =  768*1024, // 768 KB
  RIV_MEMSIZE_FRAMEBUFFER  = 1024*1024, // 1 MB
} riv_mem_size;

typedef enum riv_mmio_offset {
  RIV_MMIOSTART_MMIO_DRIVER  = 0,
  RIV_MMIOSTART_MMIO_DEVICE  = RIV_MEMSIZE_MMIO_DRIVER,
  RIV_MMIOSTART_AUDIOBUFFER  = RIV_MEMSIZE_MMIO_DRIVER + RIV_MEMSIZE_MMIO_DEVICE,
  RIV_MMIOSTART_FRAMEBUFFER  = RIV_MEMSIZE_MMIO_DRIVER + RIV_MEMSIZE_MMIO_DEVICE + RIV_MEMSIZE_AUDIOBUFFER,
} riv_mmio_offset;

typedef enum riv_vaddr_base {
  RIV_VADDR_BASE         = 0x10000000,
  RIV_VADDR_MMIO_DRIVER  = RIV_VADDR_BASE + RIV_MMIOSTART_MMIO_DRIVER,
  RIV_VADDR_MMIO_DEVICE  = RIV_VADDR_BASE + RIV_MMIOSTART_MMIO_DEVICE,
  RIV_VADDR_AUDIOBUFFER  = RIV_VADDR_BASE + RIV_MMIOSTART_AUDIOBUFFER,
  RIV_VADDR_FRAMEBUFFER  = RIV_VADDR_BASE + RIV_MMIOSTART_FRAMEBUFFER,
} riv_vaddr_base;

typedef enum riv_fps_limit {
  RIV_FPS_INVALID = 0,
  RIV_FPS_24 = 24,
  RIV_FPS_30 = 30,
  RIV_FPS_48 = 48,
  RIV_FPS_60 = 60,
  RIV_FPS_UNLIMITED = 0xffffffff,
} riv_fps_limit;

typedef enum riv_constants {
  RIV_DEFAULT_WIDTH = 256,
  RIV_DEFAULT_HEIGHT = 256,
  RIV_DEFAULT_FPS = 60,
} riv_constants;

typedef struct riv_span_uint8 {
  uint8_t* data;
  uintptr_t size;
} riv_span_uint8;

////////////////////////////////////////////////////////////////////////////////
// Structures used by both device and driver

typedef struct riv_framebuffer_desc {
  uint32_t height;
  uint32_t width;
  uint32_t fps;
  riv_pixel_format pixel_format;
} riv_framebuffer_desc;

typedef struct riv_audio_ctl_desc {
  riv_audio_command command;
  uint32_t flags;
  uint64_t handle_id;
  uint32_t buffer_offset;
  uint32_t data_size;
  uint32_t volume;
} riv_audio_ctl_desc;

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
  riv_audio_ctl_desc audio_ctl;
  riv_framebuffer_desc framebuffer_desc;
  bool tracked_keys[128];
  uint32_t palette[256];
} riv_mmio_driver;

// Only device writes, only driver reads.
typedef struct riv_mmio_device {
  riv_mmio_header header;
  uint32_t key_event_count;
  uint8_t key_events[128];
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
  // private fields
  riv_mmio_driver* mmio_driver;
  riv_mmio_device* mmio_device;
  int32_t yield_fd;
  // public read-only fields
  riv_key_state keys[128];
  uint64_t frame;
  // public read/write fields
  bool running;
  riv_framebuffer_desc* framebuffer_desc;
  uint8_t* framebuffer;
  uint8_t* audiobuffer;
  bool* tracked_keys;
  uint32_t* palette;
} riv_context;

typedef void (*riv_context_callback)(riv_context*);

typedef struct riv_run_desc {
  riv_context_callback init_cb;
  riv_context_callback cleanup_cb;
  riv_context_callback frame_cb;
  riv_framebuffer_desc framebuffer_desc;
} riv_run_desc;

////////////////////////////////////////////////////////////////////////////////
// API

// utils
uint64_t riv_version(void);
uint64_t riv_rdcycle(void);
int32_t riv_printf(char* format, ...);

// context
void riv_init(riv_context* self);
void riv_uninit(riv_context* self);
void riv_ctl(riv_context* self, riv_control_reason reason);
void riv_present(riv_context* self);
void riv_loop(riv_context* self, riv_context_callback frame_cb);
uint64_t riv_sound_play_from_memory(riv_context* self, riv_span_uint8 data, uint32_t vol);
void riv_sound_stop(riv_context* self, uint64_t sound_id);
void riv_run(riv_run_desc* run_desc);
void riv_stdrun(riv_context_callback frame_cb);

// prng
uint64_t riv_rand(riv_prng* self);
uint64_t riv_rand_uint(riv_prng* self, uint64_t high);
int64_t riv_rand_int(riv_prng* self, int64_t low, int64_t high);
double riv_rand_float(riv_prng* self);
void riv_seed(riv_prng* self, uint64_t a, uint64_t b);

#endif
