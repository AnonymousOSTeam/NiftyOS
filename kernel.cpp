// kernel.cpp
#include <stdint.h> // Add this for uint32_t

// Correct the vbe_mode_info struct (full definition)
struct vbe_mode_info {
    uint16_t attributes;
    uint16_t window_a;
    uint16_t window_b;
    uint32_t win_granularity;
    uint32_t win_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr;
    uint16_t pitch;
    uint16_t width;
    uint16_t height;
    uint8_t w_char;
    uint8_t y_char;
    uint8_t planes;
    uint8_t bpp;
    uint8_t banks;
    uint8_t memory_model;
    uint8_t bank_size;
    uint8_t image_pages;
    uint8_t reserved0;
    uint32_t framebuffer;
} __attribute__((packed));

// Declare global variables
volatile uint32_t* framebuffer;
uint32_t screen_width, screen_height, screen_pitch;

extern "C" void kernel_main(uint32_t magic, uint32_t* multiboot_info) {
    // ... rest of code ...
}
