/*
    Places the interpreter around DOS 3.3's RWTS, which Zork's boot
    loader leaves at $2400-$2BFF. `main` is first at $0800, where the
    boot loader jumps and where restart() goes. The code that runs on
    every instruction follows it, up to $21FF, the end of the space
    Zork's own interpreter has. The rest of the program, its data, and
    the page tables go from $2C00, and the story file is loaded at the
    first page after them, __story. The page tables start on a page,
    as Zork's do, so that none of the four 128-byte tables crosses a
    page and an indexed read of one never takes the extra cycle for
    crossing. dasm stops with a message if a region overflows.
*/
MEMORY
{
    low (rwx) : ORIGIN = 0x0800, LENGTH = 0x2200 - 0x0800
    high (rwx) : ORIGIN = 0x2C00, LENGTH = 0xC000 - 0x2C00
}

SECTIONS
{
    .main : { *(.text.main) } > low
    .text : {
        decode.asm(.text .text.*)
        pages.asm(.text .text.*)
        zstring.asm(.text .text.*)
        output.asm(.text .text.*)
        objects.asm(.text .text.*)
        numbers.asm(.text .text.*)
    } > low
    .text.rest : { *(.text .text.*) } > high
    .rodata : { *(.rodata .rodata.*) } > high
    .data : { *(.data .data.*) } > high
    .bss : { *(.bss .bss.*) } > high
    .tables ALIGN(256) : { *(.tables) } > high
    __story = ALIGN(., 256);
}
