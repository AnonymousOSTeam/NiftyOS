 uint32_t* back_buffer = (uint32_t*)0x200000; //Allocate memory
 void swap_buffers() {
     memcpy(framebuffer, back_buffer, screen_pitch * screen_height);
 }

