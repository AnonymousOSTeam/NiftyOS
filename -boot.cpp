	.file	"boot.cpp"
	.text
#APP
	.global stack_bottom
stack_bottom:
    .quad stack_data
.global stack_top
stack_top:
    .quad stack_data + 16384

	.size _start, . - _start
#NO_APP
	.p2align 4
	.globl	_start
	.type	_start, @function
_start:
.LFB0:
	.cfi_startproc
#APP
# 60 "boot.cpp" 1
	        /*
        The bootloader has loaded us into 32-bit protected mode on a x86 
        machine. Interrupts are disabled. Paging is disabled. The processor 
        state is as defined in the multiboot standard. The kernel has full control of the CPU. The kernel can only make use of hardware features and any code it provides as part of itself. There's no printf 
        function, unless the kernel provides its own <stdio.h> header and a 
        printf implementation. There are no security restriction, no 
        safeguards, no debugging mechanisms, only what the kernel provides itself. It has absolute and complete power over the machine.
        */

        /*
        To set up a stack, we set the esp register to point to the top of the
        stack (as it grows downwards on x86 systems). This is necessarily done
        in assembly as languages such as C cannot function without a stack. 
        */
        mov $stack_top, %esp

        /*
        This is a good place to initialize crucial processor state before the high-level kernel is entered. It's best to minimize the early environment where crucial features are offline. Note that the processor is not fully initialized yet: Features such as floating point instructions and instruction set extensions are not initialized 
        yet. The GDT should be loaded here. Paging should be enabled here. 
        C++ features such as global constructors and exceptions will require runtime support to work as well. 
        */

        /*
        Enter the high-level kernel. The ABI requires the stack is 16-byte
        aligned at the time of the call instruction (which afterwards pushes
        the return pointer of size 4 bytes). The stack was originally 16-byte 
        aligned above and we've pushed a multiple of 16 bytes to the 
        stack since (pushed 0 bytes so far), so the alignment has thus been
        preserved and the call is well defined.
        */
        call kernel_main

        /*
        If the system has nothing more to do, put the computer into an infinite loop. to do that:
        1) Disable interrupts with cli (clear interrupt enable in eflags).
           They are already disabled by the bootloader, so this is not needed. 
           Mind that you might later enable interrupts and return from 
           kernel_main (which is sort of nosensical to do).
        2) Wait for the next interrupt to arrive with hlt (halt instruction).
           Since they are disabled, this will lock up the computer.
        3) Jump to the hlt instruction if it ever wakes up due to a 
           non-maskable interrupt occurring or due to system management mode.
        */
        cli
1:      hlt
        jmp 1b

        /*
        Set the size of the _start symbol to the current location '.' minus its start.
        This is useful when debugging or when you implement call tracing.
        */

# 0 "" 2
#NO_APP
	ud2
	.cfi_endproc
.LFE0:
	.size	_start, .-_start
	.globl	stack_data
	.section	.bss,"aw",@nobits
	.align 16
	.type	stack_data, @object
	.size	stack_data, 16384
stack_data:
	.zero	16384
	.section	.multiboot,"a"
	.align 4
	.type	_ZL16multiboot_header, @object
	.size	_ZL16multiboot_header, 12
_ZL16multiboot_header:
	.long	464367618
	.long	3
	.long	-464367621
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
