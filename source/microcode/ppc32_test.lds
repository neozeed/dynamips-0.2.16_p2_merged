SECTIONS {
        .text 0xfff00000 : { *(.text) }
        .rodata : { *(.rodata) }
        .data 0xfff043b8 : { *(.data) }
        .bss 0x00004000 : { *(.bss)  *(COMMON) }
}

