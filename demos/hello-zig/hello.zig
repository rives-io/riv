const riv = @cImport({
    @cInclude("riv.h");
});

pub fn main() !void {
    while (riv.riv_present()) {
        riv.riv_clear(0);
        _ = riv.riv_draw_text("hello world!", riv.RIV_SPRITESHEET_FONT_5X7, riv.RIV_CENTER, 128, 128, 2, riv.RIV_COLOR_WHITE);
    }
}
