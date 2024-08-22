use std::ffi::CString;
#[allow(
    non_snake_case,
    non_camel_case_types,
    non_upper_case_globals,
    dead_code,
    unused_imports
)]
mod riv;
use riv::*;

fn main() {
    unsafe {
        let image_path = CString::new("assets/ferris.png").expect("CString::new failed");
        let text = CString::new("hello rust!").expect("CString::new failed");

        let img_id = riv_make_image(image_path.as_ptr(), 0);

        loop {
            riv_clear(0);
            riv_draw_text(
                text.as_ptr(),
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
