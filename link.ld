ENTRY(kernel_main)
SECTIONS {
    . = 0x1000;
    .text  : { *(.text.kernel_main) *(.text) }
    .data  : { *(.rodata) *(.data) }
    .bss   : { . = ALIGN(4096); *(.bss) }
}
