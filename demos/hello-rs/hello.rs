#![no_main] // By not using standard Rust main entrypoint greatly reduces binary size.
#[allow(non_camel_case_types,dead_code,unused_imports)]
mod riv;
use riv::*;

#[no_mangle]
unsafe extern fn main() {
    unsafe {
        let img_id = riv_make_image(c"assets/ferris.png".as_ptr(), 0);
        loop {
            riv_clear(riv_color_id::RIV_COLOR_BLACK as u32);
            riv_draw_text(
                c"hello rust!".as_ptr(),
                riv_spritesheet_id::RIV_SPRITESHEET_FONT_5X7 as u64,
                riv_align::RIV_CENTER,
                128,
                30,
                2,
                riv_color_id::RIV_COLOR_WHITE as i64,
            );
            riv_draw_image_rect(img_id, 8, 10, 256, 256, 0, 0, 1, 1);
            if !riv_present() {
                break;
            }
        }
    }
}
