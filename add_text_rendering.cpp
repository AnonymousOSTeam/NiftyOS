 // font.cpp 
 extern const uint8_t font_0x8[256][8]; // Define or load a font

 void draw_char(uint32_t x, uint32_t y, char c, uint32_t color) {
     for (uint32_t dy = 0; dy < 8; dy++)
         for (uint32_t dx = 0; dx < 8; dx++)
             if (font_8x8[(uint8_t)c][dy] & (1 << dx))
                 draw_pixel(x + dx, y + dy, color);
 }

 void draw_string(uint32_t x, uint32_t y, const char* str, uint32_t color) {
     while (*str) {
         draw_char(x, y, *str++, color);
         x += 8;
     }
}

