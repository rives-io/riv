extern "C" {
    pub fn riv_present() -> bool;
    pub fn riv_clear(col: u32) -> bool;
}

fn main() {
    while unsafe { riv_present() } {
        unsafe { riv_clear(1); }
    }
}