; boot.s
.section .multiboot
    .long 0x1BADB002   ; Magic number (required)
    .long 0x00         ; Flags (0 = no additional features)
    .long -(0x1BADB002 + 0x00) ; Checksum (magic + flags + checksum = 0)

.section .text
.global _start
_start:
    call kernel_main   ; Call your C++ kernel
    cli
    hlt
