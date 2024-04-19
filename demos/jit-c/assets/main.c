//usr/bin/riv-jit-c "$0" "$@"; exit $?

#include <riv.h>

int main(int argc, char** argv) {
    do {
        riv_clear(RIV_COLOR_DARKSLATE);
        riv_draw_text("HELLO FROM C JIT!", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, 128, 128, 1, RIV_COLOR_WHITE);
    } while(riv_present());
    return 0;
}
