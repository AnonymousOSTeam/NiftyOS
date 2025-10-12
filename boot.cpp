#include <stdint.h>

/* Declare constants for the multiboot header. */
static constexpr uint32_t ALIGN   = 1 << 0;             /* align loaded modules on page boundaries */
static constexpr uint32_t MEMINFO = 1 << 1;             /* provide memory map */
static constexpr uint32_t FLAGS   = ALIGN | MEMINFO;    /* this is the Multiboot 'flag' field */
static constexpr uint32_t MAGIC   = 0x1BADB002;         /* 'magic number' lets bootloader find the header */
static constexpr int32_t  CHECKSUM = -(MAGIC + FLAGS);   /* checksum of above, to prove we are multiboot */

/*
Declare a multiboot header that marks the program as a kernel. These are magic
values that are documented in the multiboot standard. The bootloader will 
search for this signature in the first 8 KiB of the kernel file, aligned at a 
32-bit boundary. The signature is in its own section so the header can be forced to be within the first 8 KiB of the kernel file. 
*/
extern "C" {
__attribute__((section(".multiboot"), used, aligned(4)))
const uint32_t multiboot_header[3] = { MAGIC, FLAGS, static_cast<uint32_t>(CHECKSUM) };
}

/*
The multiboot standard does not define the value of the stack pointer register (esp) and it is up to the kernel to provide a stack. This allocates room for a small stack by creating a symbol at the bottom of it, then allocating 16384 
bytes for it, and finally creating a symbol at the top. The stack grows 
downwards on x86. The stack is in its own section so it can be marked nobits, 
uninitialized stack. The stack on x86 must be 16-byte aligned according to the 
System V ABI standard and de-facto extensions. The compiler will assume the 
stack is properly aligned and failure to align the stack will result in 
undefined behavior.
*/
extern "C" {
    /* Allocate 16 KiB stack in the .bss section, aligned to 16 bytes */
    __attribute__((section(".bss"), used, aligned(16)))
    unsigned char stack_data[16384];
}

/* 
Create symbols for stack_bottom and stack_top corresponding to the beginning
and end of the allocated stack. 
*/
asm (
    ".global stack_bottom\n"
    "stack_bottom:\n"
    "    .quad stack_data\n"
    ".global stack_top\n"
    "stack_top:\n"
    "    .quad stack_data + 16384\n"
);

/*
The linker script specifies _start as the entry point to the kernel and the bootloader will jump to this position once the kernel has been loaded. It
doesn't make sense to return from this function as the bootloader is gone. 
*/
extern "C" {

    /* Forward declaration of kernel_main */
    void kernel_main();

    /* _start: The kernel entry point. */
    __attribute__((naked)) void _start() {
        __asm__ volatile (
            "        /*\n"
            "        The bootloader has loaded us into 32-bit protected mode on a x86 \n"
            "        machine. Interrupts are disabled. Paging is disabled. The processor \n"
            "        state is as defined in the multiboot standard. The kernel has full control of the CPU. The kernel can only make use of hardware features and any code it provides as part of itself. There's no printf \n"
            "        function, unless the kernel provides its own <stdio.h> header and a \n"
            "        printf implementation. There are no security restriction, no \n"
            "        safeguards, no debugging mechanisms, only what the kernel provides itself. It has absolute and complete power over the machine.\n"
            "        */\n"
            "\n"
            "        /*\n"
            "        To set up a stack, we set the esp register to point to the top of the\n"
            "        stack (as it grows downwards on x86 systems). This is necessarily done\n"
            "        in assembly as languages such as C cannot function without a stack. \n"
            "        */\n"
            "        mov $stack_top, %esp\n"
            "\n"
            "        /*\n"
            "        This is a good place to initialize crucial processor state before the high-level kernel is entered. It's best to minimize the early environment where crucial features are offline. Note that the processor is not fully initialized yet: Features such as floating point instructions and instruction set extensions are not initialized \n"
            "        yet. The GDT should be loaded here. Paging should be enabled here. \n"
            "        C++ features such as global constructors and exceptions will require runtime support to work as well. \n"
            "        */\n"
            "\n"
            "        /*\n"
            "        Enter the high-level kernel. The ABI requires the stack is 16-byte\n"
            "        aligned at the time of the call instruction (which afterwards pushes\n"
            "        the return pointer of size 4 bytes). The stack was originally 16-byte \n"
            "        aligned above and we've pushed a multiple of 16 bytes to the \n"
            "        stack since (pushed 0 bytes so far), so the alignment has thus been\n"
            "        preserved and the call is well defined.\n"
            "        */\n"
            "        call kernel_main\n"
            "\n"
            "        /*\n"
            "        If the system has nothing more to do, put the computer into an infinite loop. to do that:\n"
            "        1) Disable interrupts with cli (clear interrupt enable in eflags).\n"
            "           They are already disabled by the bootloader, so this is not needed. \n"
            "           Mind that you might later enable interrupts and return from \n"
            "           kernel_main (which is sort of nosensical to do).\n"
            "        2) Wait for the next interrupt to arrive with hlt (halt instruction).\n"
            "           Since they are disabled, this will lock up the computer.\n"
            "        3) Jump to the hlt instruction if it ever wakes up due to a \n"
            "           non-maskable interrupt occurring or due to system management mode.\n"
            "        */\n"
            "        cli\n"
            "1:      hlt\n"
            "        jmp 1b\n"
            "\n"
            "        /*\n"
            "        Set the size of the _start symbol to the current location '.' minus its start.\n"
            "        This is useful when debugging or when you implement call tracing.\n"
            "        */\n"
        );
    }
}

/* 
Set the size of the _start symbol to the current location '.' minus its start.
This is useful when debugging or when you implement call tracing.
*/
asm(".size _start, . - _start");

