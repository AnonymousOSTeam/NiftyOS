// gfx.cpp
#include <stdint.h> // Add this header

// Declare external variables from kernel.cpp
extern volatile uint32_t* framebuffer;
extern uint32_t screen_pitch;

void draw_pixel(uint32_t x, uint32_t y, uint32_t color) {
    framebuffer[y * (screen_pitch / 4) + x] = color;
}

void draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
    for (uint32_t dy = 0; dy < h; dy++)
        for (uint32_t dx = 0; dx < w; dx++)
            draw_pixel(x + dx, y + dy, color);
}
