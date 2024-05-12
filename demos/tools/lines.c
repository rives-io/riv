//riv-jit-c
#include <riv.h>
int main() {
  do {
    uint64_t frame = riv->frame; // frame counter, incremented at every frame
    uint8_t *framebuffer = riv->framebuffer; // array to the frame buffer
    // animate diagonal lines filled with the color palette
    for(uint64_t y = 0; y < 256; y++) {
      for(uint64_t x = 0; x < 256; x++) {
        framebuffer[((y * 256) + x)] = ((((frame + x) + y) / 8) % 32);
      }
    }
  } while(riv_present()); // keep presenting the framebuffer to the screen
  return 0;
}
